/*
	Calculator.cpp
	This file contains all of the function definitions for the StackInterface.
	These will be used to generate the text interface for the stack

	Functions:
	Title			  		Description
	------------------------------------------
*/

/*----------Includes---------*/
#include <string>
#include <iostream>
#include <regex>  // check if something is numeric with a "regular expression" (coming up later)

#include "Calculator.h"

// Note that we don't want to write both 'using namespace std' and 'using namespace HaverfordCS' because that would bring in two different definitions of lists
// Thus, it is o.k. to 'uncomment' _one_ of the following two lines, but don't do _both_.
// using HaverfordCS::list;	// like using namespace HaverfordCS, but just for one name
// using std::list;

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#include "Dictionary.h"
#include "streams.h"  /* for "trace" output */

std::ostream &prompt = std::cerr; // this is usually visible even if cout has been captured

using std::string;

bool looksLikeInt(string s)
{
	std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
	return std::regex_match(s, intPattern);
}

bool validVarName(string s)
{
    std::regex intPattern("[A-Za-z_][A-Za-z_0-9]*"); // C-style names
    return std::regex_match(s, intPattern) and (s != "var" and s != "const");
}

/*
 * Basic Idea for putting variables and constants into one dictionary:
 *   Names must not start with a space (cin won't do it, and they won't count as "validVarName" above)
 *   So, we could enter a constant x into the dictionary as " x", or a variable as "x", look up both ways,
 *       but only overwrite if it's there _without_ the space.
 */

// evalToken will evalueate (i.e., get the value of) either a number (via stoi) or name (via the dictionary);
//  an invalid name shouldn't happen, but will throw "eval: invalid name "+nameOrNumber as a string
//  lookup will throw a different message, about using a name not in the dictionary, if it's not there
int evalToken(const Dictionary &d, const string &nameOrNumber)
{
    if (looksLikeInt(nameOrNumber)) {
        return stoi(nameOrNumber);
    } else if (validVarName(nameOrNumber)) {
        if (d.contains(" "+nameOrNumber)) {
            return d.lookup(" "+nameOrNumber);
        } else {
            return d.lookup(nameOrNumber);
        }
    } else {
        throw "eval: invalid token: "+nameOrNumber;
    }
}

bool runCalculator()
{
	string promptText = " C> ";
	string token1, token2, token3, hopeForSemicolon;
	Dictionary dict = Dictionary();
	const int maxP = 7;
	int patience = maxP;
	string complaints[maxP] = {
	        "That's it, I've had it with you and your variable names",
            "Nice try, hosehead, but not good enough :-P'''''",
            "Illegal variable name again (is there something wrong with you or your keyboard?)",
            "Illegal variable name again",
            "Illegal variable name again",
            "Illegal variable name again",
            "Illegal variable name (use only C-style variable names, e.g., letters)"
	};

	cout << "Welcome to the awesome CS245 desk calculator," << endl;
	cout << "An amazing app by N. Swerki.";
	// cout << " (with additional help from ******* and ******* and ******)"
	cout << endl;
	cout << "Enter input for the calculator, and 'bye' when you're done" << endl;
	prompt << promptText;
	while (cin and cin >> token1 and token1 != "bye") { // note cin counts as false if it can't get input
	    if (token1 == ";") continue;  // just goes right back to the "while" line, like an upside-down "break"
	    else if (token1 == "?") {
	        trace << "Printing dictionary:" << endl;
	        cout << "Sorry, no time to write a really pretty dictionary printer that skips duplicates :-(" << endl;
	        cout << dict.toCode() << endl;
	    } else {
	        try { // this can fail due to bad input at various points, they'll all "throw" an error that we catch below.
                cin >> token2;  // might want to notice ";" here or next :-P
                cin >> token3;
                if (!cin) throw string("Unexpected end of input!");
                if (token1 == "var" || token1 == "const") {
                    if (!validVarName(token2)) {
                        cerr << complaints[--patience] + ": " + token2 << endl;
                        if (patience <= 0) return false;
                    } else if (dict.contains(token2) || dict.contains(" "+token2)) {
                        cerr << complaints[--patience] + ", " + token2 + " has already been defined." << endl;
                        if (patience <= 0) return false;
                    } else {
                        string addName = (token1 == "const" ? " " : "") + token2;
                        dict = Dictionary(dict, addName, evalToken(dict, token3));
                    }
                } else if (token1 == ":=") {
                    if (!dict.contains(token2)) {
                        cerr << "Sorry, := only works for defined \"var\"s, can't assign to " + token2 + "." << endl;
                        if (--patience <= 0) {
                            cerr << "Too many errors, giving up" << endl;
                            return false;
                        }
                    } else {
                        dict = Dictionary(dict, token2, evalToken(dict, token3)); // rely on dictionary to keep most recent
                    }
                } else if (token2 == "+") {
                    cout << evalToken(dict, token1) + evalToken(dict, token3) << endl;
                } else if (token2 == "-") {
                    cout << evalToken(dict, token1) - evalToken(dict, token3) << endl;
                } else if (token2 == "*") {
                    cout << evalToken(dict, token1) * evalToken(dict, token3) << endl;
                } else if (token2 == "==") {
                    cout << ((evalToken(dict, token1) == evalToken(dict, token3))?"true":"false") << endl;
                } else if (token2 == "<=") {
                    cout << ((evalToken(dict, token1) <= evalToken(dict, token3))?"true":"false") << endl;
                } else {
                    throw "Unrecognized operator: " + token2;
                }
            } catch (string s) {
	            cerr << "oops, got illegal input (error \"" << s << "\"), let's skip to next ;..." << endl;
	        }
		}

        while (cin >> hopeForSemicolon and hopeForSemicolon != ";") {
            cerr << "unexpected " << hopeForSemicolon << " ignored, looking for ';'" << endl;
        }
		prompt << promptText;
	}
	return true;
}
