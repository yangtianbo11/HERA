#include "eval.h"
#include "hc_list_helpers.h"
#include "streams.h"
#include <regex>  // check if something is numeric with a "regular expression" (coming up later)
#include <typeinfo> //https://stackoverflow.com/questions/11310898/how-do-i-get-the-type-of-a-variable
#include "TreeHelpers.h"
#include "Dictionary.h"

using namespace HaverfordCS;
using std::string;

#if defined TREE_H_

//static Dictionary constructDict();
static Dictionary *pVar = new Dictionary();

static bool looksLikeInt(string s) {
    std::regex intPattern("-?[0-9]+"); // optional minus followed by at least one numeral
    return std::regex_match(s, intPattern);
}

static bool looksLikeBool(Tree t){
    int result;
    try {
        result = eval(t);
    }catch (int n) {
        return true;
    }
    return false;
}

static bool specialExpression(string s){
    if(looksLikeInt(s)
    || s == " call "
    || s == "+"
    || s == "-"
    || s == "*"
    || s == "="
    || s == "<="
    || s == ">="
    || s == "if"
    || s == "let*") {
        return true;
    }
    return false;
}

int eval(Tree expression) {
    trace << "Entering eval with tree whose root is " << expression.value() << std::endl;

    if (looksLikeInt(expression.value())) {
        return stoi(expression.value()); // http://www.cplusplus.com/reference/string/stoi/
    } else if (expression.value() == " call ") {
        if (expression.nChildren() < 1) {
            std::cerr << "Compiler inconsistency: Illegal function call node" << std::endl;
            exit(33);
        }
        if (expression.child(0).value() == "exit") {
            trace << "Exiting" << std::endl;
            exit(0);
        } else if (expression.child(0).value() == "getint") {
            int nextInput;
            if (std::cin >> nextInput) {
                return nextInput;
            } else {
                std::cerr << "evaluation of 'getint' failed due to no input" << std::endl;
                exit(107);
            }
        } else if (pVar->contains(expression.child(0).value())) {
            return pVar->lookup(expression.child(0).value());
        } else {
            std::cerr << "Unrecognized function call to " << expression.child(0).value() << std::endl;
            exit(34);
        }
    } else if (expression.value() == "+") {
        if (expression.nChildren() != 2) {
            std::cerr << "Oops, invalid parse tree, has more than two children in '+' node" << std::endl;
            exit(101);
        } else {
            if (looksLikeBool(expression.child(0))){
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            if (looksLikeBool(expression.child(1))){
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }
            /*int result1;
            try {
                result1 = eval(expression.child(0));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            int result2;
            try {
                 result2 = eval(expression.child(1));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }*/
            return eval(expression.child(0)) + eval(expression.child(1));
        }
        /* alternate approach:
           return foldr<int, int>(std::plus<int>(),
           0,
           map<Tree, int>(eval, expression.children()));
        */
    } else if (expression.value() == "*") {
        if (expression.nChildren() != 2) {
            std::cerr << "Oops, invalid parse tree, has more than two children in '*' node" << std::endl;
            exit(101);
        } else {
            if (looksLikeBool(expression.child(0))){
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            if (looksLikeBool(expression.child(1))){
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }
            /*int result1;
            try {
                result1 = eval(expression.child(0));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            int result2;
            try {
                 result2 = eval(expression.child(1));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }*/
            return eval(expression.child(0)) * eval(expression.child(1));
        }
    } else if (expression.value() == "-") {
        if (expression.nChildren() != 2) {
            std::cerr << "Oops, invalid parse tree, has more than two children in '-' node" << std::endl;
            exit(101);
        } else {
            if (looksLikeBool(expression.child(0))){
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            if (looksLikeBool(expression.child(1))){
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }
            /*int result1;
            try {
                result1 = eval(expression.child(0));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            int result2;
            try {
                 result2 = eval(expression.child(1));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }*/
            return eval(expression.child(0)) - eval(expression.child(1));
        }
    } else if (expression.value() == "=") {
        if (expression.nChildren() != 2) {
            std::cerr << "Oops, invalid parse tree, has more than two children in '=' node" << std::endl;
            exit(101);
        } else {
            int result1;
            try {
                result1 = eval(expression.child(0));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            int result2;
            try {
                result2 = eval(expression.child(1));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }
            if (result1==result2) {
                throw 1;
            } else {
                throw 0;
            }
        }
    } else if (expression.value() == "<=") {
        if (expression.nChildren() != 2) {
            std::cerr << "Oops, invalid parse tree, has more than two children in '<=' node" << std::endl;
            exit(101);
        } else {
            int result1;
            try {
                result1 = eval(expression.child(0));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0)) << std::endl;
                exit(101);
            }
            int result2;
            try {
                result2 = eval(expression.child(1));
            }catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1)) << std::endl;
                exit(101);
            }
            if (result1<=result2) {
                throw 1;
            } else {
                throw 0;
            }
        }
    } else if (expression.value() == ">=") {
        if (expression.nChildren() != 2) {
            std::cerr << "Oops, invalid parse tree, has more than two children in '>=' node" << std::endl;
            exit(101);
        } else {
            int result1;
            try {
                result1 = eval(expression.child(0));
            } catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(0))
                          << std::endl;
                exit(101);
            }
            int result2;
            try {
                result2 = eval(expression.child(1));
            } catch (int n) {
                std::cerr << "Oops, invalid parse tree, this child is not a number:" << toCode(expression.child(1))
                          << std::endl;
                exit(101);
            }
            if (result1 >= result2) {
                throw 1;
            } else {
                throw 0;
            }
        }
    } else if (expression.value() == "if") {
        if (looksLikeBool(expression.child(0))){
            if (looksLikeBool(expression.child(1)) == looksLikeBool(expression.child(2))){
                try {
                    eval(expression.child(0));
                }catch (int cond) {
                    if (cond==1){
                        return eval(expression.child(1));
                    } else {
                        return eval(expression.child(2));
                    }
                }
            } else {
                std::cerr << "Oops, invalid parse tree, second and third sub-expressions do not evaluate to the same type"
                          << std::endl;
                exit(101);
            }
        }
        std::cerr << "Oops, invalid parse tree, conditional expression is not a boolean"
                  << std::endl;
        exit(101);
    } else if (expression.value() == "#t" || expression.value() == "#T") {
        throw 1;
    } else if (expression.value() == "#f" || expression.value() == "#F") {
        throw 0;
    } else if (expression.value() == "let*") {
        for (int i=0; i<expression.child(0).nChildren(); i++) {
            string variableName = expression.child(0).child(i).value();
            string variableValue = expression.child(0).child(i).child(0).value();
            bool result1 = (typeid(expression.child(0).child(i).value()) == typeid((std::string)"s"));
            bool result2 = (specialExpression(expression.child(0).child(i).value()) == false);
            bool result3 = (typeid(eval(expression.child(0).child(i).child(0))) == typeid(1));
            bool result4 = (typeid((std::string)"y") == typeid((std::string)"s"));

            if (typeid(expression.child(0).child(i).value()) == typeid((std::string )"s")
                && specialExpression(expression.child(0).child(i).value()) == false
                && typeid(eval(expression.child(0).child(i).child(0))) == typeid(1)) {
                *pVar = Dictionary(*pVar, expression.child(0).child(i).value(), eval(expression.child(0).child(i).child(0)));
            } else {
                std::cerr << "Oops, invalid parse tree, improper variable definitions in let*"
                          << std::endl;
                exit(101);
            }
        }
        return eval(expression.child(1));
    } else {
        throw "Unrecognized operator: " + expression.value();
    }
}

/*static Dictionary constructDict(Dictionary d, Tree t){
    for (int i=0; i<t.nChildren(); i++) {
        if (typeid(t.child(i).value()) == typeid("s")
         && typeid(t.child(i).child(0).value()) == typeid("s")) {
            d = Dictionary(d, t.child(i).value(), eval(t.child(i).child(0)));
        } else {

        }
    }
}*/

#else
int eval(bool parseResult)
{
    return parseResult ? 1 : 0;
}
#endif


