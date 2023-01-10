/*
	main.cc
	This file contains the main function for the Dictionary project, which implements a calculator

	Run from within C-Lion, or, after building, run on the command line with
	  cmake-build-debug/Dictionary_C++ 
	or, to suppress "trace" output, use
	  TRACE_OUTPUT=/dev/null cmake-build-debug/Dictionary_C++ 
*/

/*-----------Includes---------*/
#include <iostream>

#include "listDemos.h"
#include "listPractice.h"
#include "test-Dictionary.h"
#include "Calculator.h"


using std::cout;
using std::cin;
using std::endl;

#include "streams.h"   // "trace" and "debug" streames, for this project

int main ()
{
	trace << "***** First, some list demos:" << endl;
	hcListDemos();
	trace << endl << endl;  // space between tests
	stdListDemo();
	trace << endl << endl;  // space between tests
	
	trace << "***** Now, the list practice, as described in the pre-lab" << endl;
	if (!testListPractice()) cout << "oops, it returns false" << endl;
	trace << endl << endl;  // space between tests
	
	trace << "***** Now, the dictionary test" << endl;
	testDictionary();
	trace << endl << endl;  // space between tests
	
	trace << "***** Finally, the calculator program (you'll need to write this, too):" << endl;

	if (runCalculator()) {
		// success
		cout << "Thanks for using the calculator demo" << endl;
		return 0;  // report success to operating system
	} else {
		cout << "Sorry it didn't work out too well" << endl;
		return 1;  // any non-zero counts as failure
	}
}
