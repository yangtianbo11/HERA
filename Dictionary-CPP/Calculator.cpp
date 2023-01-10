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
#include <hc_list.h>
#include "Calculator.h"
#include "Dictionary.h"

// Note that we don't want to write both 'using namespace std' and 'using namespace HaverfordCS' because that would bring in two different definitions of lists
// Thus, it is o.k. to 'uncomment' _one_ of the following two lines, but don't do _both_.
// using HaverfordCS::list;	// like using namespace HaverfordCS, but just for one name
// using std::list;

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#include "streams.h"  /* for "trace" output */

std::ostream &prompt = std::cerr; // this is usually visible even if cout has been captured

using std::string;

const int nonsense = -999;

Dictionary varList = Dictionary();
Dictionary constVarList = Dictionary();

bool looksLikeInt(string s) {
    std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
    return std::regex_match(s, intPattern);
}

/* Old toInt method with fixed definitions for x and y
int toInt(string s){
    if(looksLikeInt(s)){
        return stoi(s);
    } else if(s=="x"){
        return 1;
    } else if(s=="y") {
        return 2;
    } else {
        cout << "Not a valid input" << endl;
        return nonsense;
        //throw "Not a valid input";
    }
}*/

int toInt(string s) {
    if (looksLikeInt(s)) {
        return stoi(s);
    } else if (varList.contains(s)) {
        return varList.lookup(s);
    } else {
        cout << "Not a valid input" << endl;
        return nonsense;
        //throw "Not a valid input";
    }
}

bool operate(string token1, string token2, string token3) {
    trace << "thanks for entering the integer " << toInt(token1) << endl;
    int ans;
    cin >> token2;
    trace << "thanks for entering " << token2 << endl;
    if (token2 != "+" && token2 != "-" && token2 != "*" && token2 != "==" && token2 != "<=" && token2 != ">=") {
        cerr << "Sorry, this is an invalid operation, calculator shutting down" << endl;
        return false;
    }
    cin >> token3;
    if (looksLikeInt(token3) || varList.contains(token3)) {
        trace << "thanks for entering the integer " << toInt(token3) << endl;
    } else {
        trace << "Sorry, second operand must be a number, calculator shutting down" << endl;
        return false;
    }
    if (token2 == "+") {
        ans = toInt(token1) + toInt(token3);
        cout << ans << endl;
    } else if (token2 == "-") {
        ans = toInt(token1) - toInt(token3);
        cout << ans << endl;
    } else if (token2 == "*") {
        ans = toInt(token1) * toInt(token3);
        cout << ans << endl;
    } else if (token2 == "==") {
        if (toInt(token1) == toInt(token3)) {
            ans = 1;
            cout << "True" << endl;
        } else {
            ans = 0;
            cout << "False" << endl;
        }
    } else if (token2 == "<=") {
        if (toInt(token1) <= toInt(token3)) {
            ans = 1;
            cout << "True" << endl;
        } else {
            ans = 0;
            cout << "False" << endl;
        }
    } else if (token2 == ">=") {
        if (toInt(token1) >= toInt(token3)) {
            ans = 1;
            cout << "True" << endl;
        } else {
            ans = 0;
            cout << "False" << endl;
        }
    }
    varList = Dictionary(varList, "^", ans);
    return true;
}

bool defineConstantVar(string token2, string token3) {
    trace << "Please set a new variable and its corresponding value:" << endl;
    cin >> token2;
    trace << "thanks for entering " << token2 << endl;
    if (looksLikeInt(token2) || varList.contains(token2) || token2 == "+" || token2 == "-"
        || token2 == "*" || token2 == "==" || token2 == "<=" || token2 == ">=" || token2 == "^") {
        cerr << "Sorry, invalid variable name, calculator shutting down" << endl;
        return false;
    }
    cin >> token3;
    if (looksLikeInt(token3) || varList.contains(token3)) {
        trace << "thanks for entering the integer " << toInt(token3) << endl;
    } else {
        trace << "Sorry, the value must be a number or previously defined variable, calculator shutting down" << endl;
        return false;
    }
    constVarList = Dictionary(constVarList, token2, toInt(token3));
    varList = Dictionary(varList, token2, toInt(token3));
    return true;
}

bool defineVar(string token2, string token3) {
    trace << "Please set a new variable and its corresponding value:" << endl;
    cin >> token2;
    trace << "thanks for entering " << token2 << endl;
    if (looksLikeInt(token2) || varList.contains(token2) || token2 == "+" || token2 == "-"
        || token2 == "*" || token2 == "==" || token2 == "<=" || token2 == ">=" || token2 == "^") {
        cerr << "Sorry, invalid variable name, calculator shutting down" << endl;
        return false;
    }
    cin >> token3;
    if (looksLikeInt(token3) || varList.contains(token3)) {
        trace << "thanks for entering the integer " << toInt(token3) << endl;
    } else {
        trace << "Sorry, value must be a number or previously defined variable, calculator shutting down" << endl;
        return false;
    }
    varList = Dictionary(varList, token2, toInt(token3));
    return true;
}

bool changeVar(string token2, string token3) {
    trace << "Please name an existing variable and its new value:" << endl;
    cin >> token2;
    trace << "thanks for entering " << token2 << endl;
    if (looksLikeInt(token2) || constVarList.contains(token2) || !varList.contains(token2)
        || token2 == "+" || token2 == "-" || token2 == "*" || token2 == "==" || token2 == "<="
        || token2 == ">=" || token2 == "^") {
        cerr << "Sorry, invalid variable name, calculator shutting down" << endl;
        return false;
    }
    cin >> token3;
    if (looksLikeInt(token3) || varList.contains(token3)) {
        trace << "thanks for entering the integer " << toInt(token3) << endl;
    } else {
        trace << "Sorry, value must be a number or previously defined variable, calculator shutting down" << endl;
        return false;
    }
    varList = Dictionary(varList, token2, toInt(token3));
    //varList.set(token2, toInt(token3));
    return true;
}

bool listAllVar() {
    cout << "List of all variables currently defined:" << endl;
    varList.print();
    return true;
}

bool runCalculator() {
    string promptText = " C> ";
    string token1, token2, token3, hopeForSemicolon;
    bool keepRunning = true;
    cout << "Programmers: Tianbo Yang and Issac Wasserman" << endl;
    cout << "Enter input for the calculator, and 'bye' when you're done" << endl;
    prompt << promptText;
    while (cin and cin >> token1 and token1 != "bye") { // note cin counts as false if it can't get input
        if (looksLikeInt(token1) || varList.contains(token1)) {
            keepRunning = operate(token1, token2, token3);
            if (keepRunning == false) {
                return keepRunning;
            }
        } else if (token1 == "const") {
            keepRunning = defineConstantVar(token2, token3);
            if (keepRunning == false) {
                return keepRunning;
            }
        } else if (token1 == "var") {
            keepRunning = defineVar(token2, token3);
            if (keepRunning == false) {
                return keepRunning;
            }
        } else if (token1 == ":=") {
            keepRunning = changeVar(token2, token3);
            if (keepRunning == false) {
                return keepRunning;
            }
        } else if (token1 == "?") {
            keepRunning = listAllVar();
        } else {
            trace << "thanks for entering the non-integer token " << token1
                  << ", but I don't quite know what to do with it..." << endl;
            trace << "I'll keep discarding input until the next ';'" << endl;
        }
        while (cin >> hopeForSemicolon and hopeForSemicolon != ";") {
            cerr << "unexpected " << hopeForSemicolon << " ignored, looking for ';'" << endl;
        }
        prompt << promptText;
    }
    return keepRunning;
}
