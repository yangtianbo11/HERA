#include <iostream>
#include <cstdlib>  // for 'exit' function to abandon the program
#include <hc_list_helpers.h>
#include "parser.h"
#include "scanner.h"

using std::string;
using HaverfordCS::list;
using HaverfordCS::ez_list;
using std::cout;
using std::cerr;
using std::endl;

#include "streams.h"
#include "Dictionary.h"


// match this:
// START -> E   <followed by end-of-input>
// E -> number
// E -> identifier
// E -> ( E_IN_PARENS )
// E_IN_PARENS -> OP E E
// E_IN_PARENS -> EXIT
// E_IN_PARENS -> GETINT   // New in this version, gets user input
// OP --> +|-|*| OP_COMPARE

// Declare all functions, so they can call each other in any order
//  (note: "static" means it's restricted to this scope, e.g. this file
static ParserResult matchE();
static ParserResult matchEInParens();
static string matchOp();


// FIRST and FOLLOW sets for those,
//  built with lists rather than sets, for familiarity
//  defined in terms of kindOfToken from scanner-regexp.h
static list<kindOfToken> FIRST_OP  = ez_list(PLUS, MINUS, TIMES, EQUAL, LESS_OR_EQUAL, GREATER_OR_EQUAL);
static list<kindOfToken> FIRST_EIP = FIRST_OP;
static list<kindOfToken> FIRST_E   = ez_list(INT_LITERAL, BOOL_LITERAL, IDENTIFIER, LPAREN);

static list<kindOfToken> FOLLOW_OP  = FIRST_E;
static list<kindOfToken> FOLLOW_EIP = ez_list(RPAREN);
static list<kindOfToken> FOLLOW_E   = /* append(FOLLOW_EIP, FIRST_E) */
		                              list<kindOfToken>(RPAREN, FIRST_E);
	                                  /* Dave should put "append" into list_helpers */

// first, some helpful functions:

// mustGetNextToken:
//    we call this when we need to get more input and we must find something there,
//    i.e. when we want to move current_token along and end-of-input would be an error
void mustGetNextToken()
{
	getNextToken();
	if (!tokenAvailable()) {
		cerr << "unexpected end of input at token #" << tokenNumber() << endl;
		exit(5);
	}
}
static string matchLiteral(kindOfToken whatDoYouWant)
{
    if (currentTokenKind() == whatDoYouWant) {
        string it = currentToken();
        getNextToken();
        return it;
    } else {
        std::cerr << "Parse error, got token (" << currentToken() << ") at token #" << tokenNumber() << endl;
        exit(3);
    }
}

// currentTokenThenMove
//   record currentToken (which must NOT be end-of-input), then move past it, return what it was
static std::string currentTokenThenMove()
{
	if (!tokenAvailable()) {
		cerr << "unexpected end of input at token #" << tokenNumber() << endl;
		exit(5);
	}
	std::string curr = currentToken();
	getNextToken();
	return curr;
}

// confirmLiteral
//   match a literal, assuming the token HAS been scanned already,
//    i.e. that "currentToken" is _on_ the literal we wish to match
//   leave "currentToken" on the very last token of the matched pattern ... this is not a "match" function
static void confirmLiteral(string what)
{
	if (!tokenAvailable()) {
		cerr << "unexpected end of input at token #" << tokenNumber() << endl;
		exit(5);
	}
	if (currentToken() != what) {
		cerr << "got " << currentToken() << " instead of " << what << " at token #" << tokenNumber() << endl;
		exit(2);
	}	
}
static bool convertBool(string s){
    if(s == "#t"
       || s == "#T")
        return true;
    else if(s == "#f" || s== "#F"){
        return false;
    }
    cerr << "convertBool receives " << s << " instead of " << "a token of bool starting with #" << endl;
    exit(2);
}


// match an "E", i.e, anything on the right hand side of any "E-->..." production
//  assume the first token of the E has been scanned
//   (i.e., assuming currentToken is the first token of the "E" we're matching)
//  leave "currentToken" AFTER the very last token of the matched pattern
static ParserResult matchE()
{
	trace << "Entering matchE, current token is " << currentToken() << endl;
	if (currentTokenKind() == INT_LITERAL) {
	        return new IntLiteralNode(std::stoi(matchLiteral(INT_LITERAL)));
	} else if (currentTokenKind() == BOOL_LITERAL) {   //**************************************
            return new BoolLiteralNode(convertBool(matchLiteral(BOOL_LITERAL)));
	} else if (currentTokenKind() == IDENTIFIER) {	
	    	return matchEInParens(); //
	} else if (currentTokenKind() == LPAREN) {
        matchLiteral(LPAREN);
	    //confirmLiteral("(");
		//mustGetNextToken();
		ParserResult it = matchEInParens();
		trace << "After matchEInParens, back in matchE, current token is: " << currentToken() << endl;
		// if that left off AFTER the end of the E_IN_PARENS, we still need a ")" in the E we're matching
        matchLiteral(RPAREN);
		//confirmLiteral(")");
		//getNextToken();	 // we're AFTER the ) now
		return it;
	} else {				
		std::cerr << "Illegal token (" << currentToken() << ") at token #" << tokenNumber() << endl;
		exit(3);
	}
}
static ParserResult matchLet(); //**************************************
// match an "E_IN_PARENS",
//  assuming that the currentToken is at the start of the E_IN_PARENS, e.g. a "+"
//  leave the currentToken AFTER the last part of what was matched,
//  i.e. *on* the ")" that should come after the E_IN_PARENS

static ParserResult matchEInParens() {
	trace << "Entering matchEInParens, current token is " << currentToken() << endl;
	if (find(currentTokenKind(), FIRST_OP)) {
		bool its_a_comparison_op = (currentTokenKind() == EQUAL || currentTokenKind() == LESS_OR_EQUAL || currentTokenKind() == GREATER_OR_EQUAL );
		string theOp = matchOp(); //**************************************
		ParserResult firstChild = matchE();
		ParserResult secondChild = matchE();
		if(firstChild->evalType() == ET_VAR || secondChild->evalType() == ET_VAR ||
		   firstChild->evalType() == ET_CALL ||secondChild->evalType() == ET_CALL ||
		   firstChild->evalType() == secondChild->evalType() && firstChild->evalType() != ET_BOOL) {
            if (its_a_comparison_op) { // a comparison
                return new ComparisonNode(theOp, firstChild, secondChild);
            } else {
                return new ArithmeticNode(theOp, ez_list(firstChild, secondChild));
            }
        }else{
            std::cerr <<"TypeError in matchEInParens: " << theOp << " token operands are not having correct types"  << endl;
            exit(3);
        }
	} else if (currentTokenKind() == IF) {    // PREDICT for EIP -> identifier
        string theIf = matchLiteral(IF); //**************************************
        ParserResult firstChild = matchE();
        ParserResult secondChild = matchE();
        ParserResult thirdChild = matchE();
        if(firstChild->evalType() == ET_BOOL && secondChild->evalType() == thirdChild->evalType())
            return new IfNode(theIf, ez_list(firstChild, secondChild, thirdChild));
        else{
            std::cerr <<"TypeError in matchEInParens: " << theIf << " token operands are not having correct types"  << endl;
            exit(3);
        }

    } else if (currentTokenKind() == LET_STAR) {    // PREDICT for EIP -> identifier
        string theLet = matchLiteral(LET_STAR); //**************************************
        ParserResult firstChild = matchLet();
        ParserResult secondChild = matchE();
        return new LetStarNode(theLet, ez_list(firstChild, secondChild));
    } else if (currentTokenKind() == IDENTIFIER && currentToken() == "exit") {
		return new CallNode(currentTokenThenMove(), list<ParserResult>());
	} else if (currentTokenKind() == IDENTIFIER && currentToken() == "getint") {
		return new CallNode(currentTokenThenMove(), list<ParserResult>());
	} else if (currentTokenKind() == IDENTIFIER){
	    string theVar = matchLiteral(IDENTIFIER);;
        //ParserResult theVal = matchE();
        return new VarUseNode(theVar, list<ParserResult>());
    } else {
		std::cerr << "Illegal token (" << currentToken() << ") at token #" << tokenNumber() << endl;
		exit(3);
	}
}
static ParserResult matchLet() { //**************************************
     if (currentTokenKind() == LPAREN){
        string dictionaryStart = matchLiteral(LPAREN); // read LPAREN
        list<ExprNode*> entries = list<ExprNode*>();
        while (currentTokenKind() != RPAREN) {
            matchLiteral(LPAREN); // read LPAREN
            string variable = matchLiteral(IDENTIFIER);// read IDENTIFIER;
            //trace << "Entering matchLet, current token is " << currentToken() << endl;
            ExprNode* pair = new VarUseNode(variable, ez_list(matchE())); //, ez_list(matchE()));

            entries = list<ExprNode*>(pair, entries);
            matchLiteral(RPAREN);  // Read RPAREN
        }
         matchLiteral(RPAREN);     // read RPAREN;
        return new LetStarNode(dictionaryStart, entries);
    }
    std::cerr << "Parse error, got token (" << currentToken() << ") at token #" << tokenNumber() << endl;
    exit(3);
}

// match an operator, assuming that it is the currentToken
//  leave the currentToken AFTER the last part of what was matched, i.e. unchanged
static string matchOp()
{
	trace << "Entering matchOp, current token is " << currentToken() << endl;
	// could do three cases here, but that's so tedious...
	assert (find(currentTokenKind(), FIRST_OP));
	return currentTokenThenMove();
}


ParserResult matchStartSymbolAndEOF()
{
	getNextToken();  // this will be the first one
	if (!tokenAvailable()) {
		cerr << "Illegal end of input" << endl;
		exit(2);
	}

	ParserResult fullExpression = matchE();  // "E" is our start symbol

	// now make sure there isn't anything else!
	getNextToken();
	if (tokenAvailable()) {
		cerr << "Warning: extra input after end: " << currentToken() << endl;
		exit (1);
	}

	return fullExpression;
}
