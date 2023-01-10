/*
 * "Interpreter" project for HaverRacket
 *  Read in and execute a subset of Racket
 *
 * To run this project and run the program tests/01-multiply.hrk, use the command:
 *   Debug/Interpreter-C++ < tests/01-multiply.hrk
 * to change trace output, run after seting HAVERRACKET_TRACE, e.g.
 *   HAVERRACKET_TRACE=tests/01-multiply-trace.out Debug/Interpreter-C++ < tests/01-multiply.hrk
 *   Note that the standard startup test of (* 6 7) can be skipped with HAVERRACKET_TEST_CODE_HERA=#f
 */

#include <iostream>
#include <cstdlib>
#include "scannerDemo.h"
#include "parser.h"
#include "eval.h"
#include "hc_list_helpers.h" // ez_list
#include "TreeHelpers.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

#include "streams.h"

#if ! defined TRACE_OUTPUT_HERE
#define TRACE_OUTPUT_HERE (getenv("HAVERRACKET_TRACE")?getenv("HAVERRACKET_TRACE"):"/dev/stderr")
#endif
#if ! defined PROMPT_OUTPUT_HERE
#define PROMPT_OUTPUT_HERE (getenv("HAVERRACKET_PROMPT")?getenv("HAVERRACKET_PROMPT"):"/dev/stderr")
#endif

#include <fstream>  /* needed for ofstream below */
std::ofstream _HaverRacket_trace(TRACE_OUTPUT_HERE);
std::ostream &trace  = _HaverRacket_trace;
std::ofstream _HaverRacket_prompt(PROMPT_OUTPUT_HERE);
std::ostream &prompt = _HaverRacket_prompt;

#if ! defined TREE_H_
// If we don't have a tree class, the parser returns true or false
typedef bool Tree;
string toCode(Tree t) { return t ? "true" : "false"; }
#endif

int main(int numberOfCommandLineArguments, char *theCommandLineArguments[])
{
	try {
		bool testScannerInstead = false;
		if (numberOfCommandLineArguments == 2 && theCommandLineArguments[1] == string("testScannerInstead")) {
			trace << "Testing Scanner instead." << endl;
			testScannerInstead = true;
		}

		if (testScannerInstead) {
			cout << "Demonstrating lexical scanner. Enter tokens followed by <EOF>." << endl;
			scannerDemo();
		} else {
#if defined TREE_H_
			if (!getenv("HAVERRACKET_TEST_EVAL") ||
			    getenv("HAVERRACKET_TEST_EVAL") == string("#t")) {
				try {
                    Tree example1 = Tree("*", HaverfordCS::ez_list(Tree("6"), Tree("7")));
					trace << "confirming 'eval' basic functionality on test tree " << toCode(example1) << endl;
					int  result   = eval(example1);
					trace << ((result == 42) ? "Good, got " : "    Uh-oh, example1 got ") << result << endl;
				} catch (std::string message) {
					cerr << "eval confirmation test threw exception: " << message << endl;
					return 2;
				}
			}
#endif
			try {
				prompt << "> "; // like Racket :-)
				prompt.flush();  // make sure it gets to the terminal! (this help the streams interleave properly)
				Tree AST = matchStartSymbolAndEOF();
				trace << "Completed Parsing, got AST: " << toCode(AST) << endl;
				try {
                    trace << "Now evaluating the input: " << endl;
                    cout << eval(AST) << endl;
                } catch (int n) {
				    if (n==0) {
                        cout << "#f" << endl;
                    } else if (n==1) {
				        cout << "#t" << endl;
				    }
				} catch (std::string message) {
					cerr << "eval threw exception (typically an unhandled case): " << message << endl;
					return 4;
				}
			} catch (std::string message) {
				cerr << "that's odd, parser threw exception: " << message << endl;
				return 3;
			}
		}

		return 0;  // indicate successful translation if no exit with non-0 before this
	} catch (...) {
		cerr << "Yikes! Uncaught exception" << endl;
		return 66;
	}
}

