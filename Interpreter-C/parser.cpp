#include <iostream>
#include <cstdlib>  // for 'exit' function to abandon the program
#include <hc_list.h>
#include <hc_list_helpers.h>
#include "Tree.h"   // should define class Tree and #define TREE_H_, though starter files do not
#include "parser.h"
#include "scanner.h"

using std::string;
using HaverfordCS::list;
using HaverfordCS::ez_list;
using std::cout;
using std::cerr;
using std::endl;
using HaverfordCS::foldl;
#include "streams.h"


// match this:
// START -> E   <end-of-input>
// E -> number                    // PREDICT = FIRST(number) = { number }
// E -> ( E_IN_PARENS )           // PREDICT = FIRST( "(" E_IN_PARENS ")" ) = { "(" }
// E_IN_PARENS -> OP E E          // PREDICT = FIRST(OP E E) = FIRST(OP) = { "+", "-", "*" }
// E_IN_PARENS -> identifier      // PREDICT = FIRST(identifier) = { identifier }   // for (exit) and (getint)
// OP -> +                        // PREDICT = { "+" }  // like the one right above, but more concise
// OP -> -                        // PREDICT = { "-" }
// OP -> *                        // PREDICT = { "*" }


#if ! defined TREE_H_
// If we don't have a class Tree, make the Tree constructors below create Booleans...
bool Tree(string s) { return true; }
bool Tree(string s, HaverfordCS::list<bool> subtrees)
{
    // fold subtrees with "and", since we want to return true if they all did
    return foldl<bool, bool>([] (bool resultSoFar, bool thisResult) { return resultSoFar and thisResult; },
                             true,
                             subtrees);
}
#endif

// FIRST (and, in principle) FOLLOW sets for each nonterminal are used to build PREDICT sets.
//  These are defined here as variables, with lists rather than a "set" class, for familiarity.
//  Note these defined in terms of kindOfToken from scanner-regexp.h, since FIRST/FOLLOW/PREDICT are sets _of_tokens_
static list<kindOfToken> FIRST_OP  = ez_list(PLUS, MINUS, TIMES, EQUAL, LESS_OR_EQUAL, GREATER_OR_EQUAL);
static list<kindOfToken> FIRST_EIP = list<kindOfToken>(IDENTIFIER, FIRST_OP); // list constructor for token and list
static list<kindOfToken> FIRST_E   = ez_list(INT_LITERAL, BOOL_LITERAL, LPAREN);

static list<kindOfToken> FOLLOW_OP  = FIRST_E;
static list<kindOfToken> FOLLOW_EIP = ez_list(RPAREN);
static list<kindOfToken> FOLLOW_E   = /* append(FOLLOW_EIP, FIRST_E) */
		                              list<kindOfToken>(RPAREN, FIRST_E);
	                                  /* @@@ Dave should put "append" into list_helpers */


// Declare all functions here at the top of the fil, so they can call each other in any order
// Each returns the appropriate abstract syntax (e.g., either a Tree (the full ParserResult) or a string)
//  (note: "static" means it's restricted to this scope, e.g. this file)
static ParserResult matchE();
static ParserResult matchEInParens();
static string matchOp();
static string matchLiteral(kindOfToken whatDoYouWant);
static ParserResult matchLet();


// match an "E", i.e, anything on the right hand side of any "E-->..." production
//  assume the first token of the E has been scanned
//   (i.e., assuming currentToken is the first token of the "E" we're matching)
//  leave "currentToken" AFTER the very last token of the matched pattern
static ParserResult matchE()
{
	trace << "Entering matchE, current token is " << currentToken() << endl;
	if (currentTokenKind() == INT_LITERAL) {    // PREDICT set for "E -> number"
        return Tree(matchLiteral(INT_LITERAL));    //  leaves "currentToken" _after_" the INT
    } else if (currentTokenKind() == BOOL_LITERAL) {    // PREDICT set for "E -> boolean"
        return Tree(matchLiteral(BOOL_LITERAL));    //  leaves "currentToken" _after_" the BOOL
    } else if (currentTokenKind() == IDENTIFIER) {
	    return matchEInParens();
	} else if (currentTokenKind() == LPAREN) {	// PREDICT set for "E -> ( E_IN_PARENS )"
		matchLiteral(LPAREN);  // don't need to save in a variable, as this isn't in the abstract syntax
		ParserResult it = matchEInParens();
		trace << "After matchEInParens, back in matchE, current token is: " << currentToken() << endl;
		// if that left off AFTER the end of the E_IN_PARENS, we still need a ")" in the E we're matching
		matchLiteral(RPAREN);
		return it;
	} else {				// if we're not in either of those "E -> " productions, we're in trouble :-(
		std::cerr << "Parse error, got token (" << currentToken() << ") at token #" << tokenNumber() << endl;
		exit(3);
	}
}

// match an "E_IN_PARENS",
//  assuming that the currentToken is at the start of the E_IN_PARENS, e.g. a "+"
//  leave the currentToken AFTER the last part of what was matched,
//  i.e. *on* the ")" that should come after the E_IN_PARENS
static ParserResult matchEInParens() {
	trace << "Entering matchEInParens, current token is " << currentToken() << endl;
	if (find(currentTokenKind(), FIRST_OP)) {         // PREDICT for EIP -> OP E E
		string theOp = matchOp();
		ParserResult firstChild = matchE();
		ParserResult secondChild = matchE();
		return Tree(theOp, ez_list(firstChild, secondChild));
	} else if (currentTokenKind() == IDENTIFIER) {    // PREDICT for EIP -> identifier
		return Tree(" call ", ez_list(Tree(matchLiteral(IDENTIFIER))));
    } else if (currentTokenKind() == IF) {    // PREDICT for EIP -> identifier
	    string theIf = matchLiteral(IF);
        ParserResult firstChild = matchE();
        ParserResult secondChild = matchE();
        ParserResult thirdChild = matchE();
        return Tree(theIf, ez_list(firstChild, secondChild, thirdChild));
    } else if (currentTokenKind() == LET_STAR) {    // PREDICT for EIP -> identifier
        string theLet = matchLiteral(LET_STAR);
        ParserResult firstChild = matchLet();
        ParserResult secondChild = matchE();
        return Tree(theLet, ez_list(firstChild, secondChild));
	} else {
		std::cerr << "Parse error, got token (" << currentToken() << ") at token #" << tokenNumber() << endl;
		exit(3);
	}
}

static ParserResult matchLet() {
    if (currentTokenKind() == LPAREN){
        string dictionaryStart = matchLiteral(LPAREN);
        list<Tree> entries = list<Tree>();
        while (currentTokenKind() != RPAREN) {
            matchLiteral(LPAREN);
            string variable = matchLiteral(IDENTIFIER);
            Tree pair = Tree(variable, ez_list(matchE()));
            entries = list<Tree>(pair, entries);
            matchLiteral(RPAREN);
        }
        matchLiteral(RPAREN);
        return Tree(dictionaryStart, entries);
    }
    std::cerr << "Parse error, got token (" << currentToken() << ") at token #" << tokenNumber() << endl;
    exit(3);
}

/*extern Dictionary *dict = new Dictionary();

static string matchDictionary() {
    trace << "Entering matchDictionary, current token is " << currentToken() << endl;
    if(currentTokenKind() == LPAREN) {
        dict= Dictionary(dict, )
    }
}*/

// match an operator, assuming that it is the currentToken
//  leave the currentToken AFTER the last part of what was matched, i.e. unchanged
static string matchOp()
{
	trace << "Entering matchOp, current token is " << currentToken() << endl;
	if (currentTokenKind() == PLUS) {        	// PREDICT for OP -> +
		return matchLiteral(PLUS);
	} else if (currentTokenKind() == MINUS) {	// PREDICT for OP -> -
		return matchLiteral(MINUS);
	} else if (currentTokenKind() == TIMES) {	// PREDICT for OP -> *
		return matchLiteral(TIMES);
    } else if (currentTokenKind() == EQUAL) {	// PREDICT for OP -> =
        return matchLiteral(EQUAL);
    } else if (currentTokenKind() == LESS_OR_EQUAL) {	// PREDICT for OP -> <=
        return matchLiteral(LESS_OR_EQUAL);
    } else if (currentTokenKind() == GREATER_OR_EQUAL) {	// PREDICT for OP -> >=
        return matchLiteral(GREATER_OR_EQUAL);
	} else {
		std::cerr << "Parse error, got token (" << currentToken() << ") at token #" << tokenNumber() << endl;
		exit(3);
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


// a couple of helpful functions:

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

