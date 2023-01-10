#if 1   // turn this to "#if 1" (or just remove this line and matched endif) to include Dictionary.h
#include "Dictionary.h"    // this had better exist, if we want this to work
#endif
#include "test-Dictionary.h"

#include <iostream>
#include "streams.h"

// change TEST_CALL_ALSO to 1 after you write and test "contains"
#define TEST_CALL_ALSO 0
#if TEST_CALL_ALSO
bool hasSamAndChris(const Dictionary &d)
{
    return d.contains("Sam") and d.contains("Chris");
}
#endif

using std::endl;
using std::cerr;

void testDictionary()
{
#if 1   // turn this to "if 1" to enable this function
	trace << "creating some dictionaries .." << endl;
	Dictionary d1 = Dictionary(); // an empty dictionary
	// Dictionary d1;  // equivalently

	Dictionary d2 = Dictionary(Dictionary(), "Sam", 42); // required
	// or  Dictionary d2 = Dictionary(d1, "Sam", 42);    // equivalent
	// or, Dictionary d2 = Dictionary().with("Sam", 42); // if you want to write that
	// or, Dictionary d2; d2.insert("Sam", 42); // if doing imperative

	trace << "let's see if \"Sam\" is home..." << endl;
	if (d1.contains("Sam")) {
		cerr << "Huh, found Sam in an empty dictionary :-(" << endl;
		// Wisconsin Tourism Federation needs to know about any problems.
	}
	if (not d2.contains("Sam")) {
		cerr << "Huh, Can't find Sam in d2 :-(" << endl;
	}

	trace << "A bigger test dictionary" << endl;
	Dictionary d3 = Dictionary(
				   Dictionary(
					      Dictionary(),
					      "Sam", 42),
				   "Chris", 17);
	// or  Dictionary d3 = Dictionary(d2, "Chris", 17);
	// or, Dictionary d3 = d2; d3.add("Chris", 17); // if doing imperative "add" method
	// or, Dictionary d3 = d2; d3 = Dictionary(d3, "Chris", 17); // imperative update of d3 without any other method

	trace << "Some checks of that bigger test dictionary, and also check the smaller ones" << endl;
	if ((not d3.contains("Sam")) or
	    (not d3.contains("Chris")) or
	    (d3.lookup("Sam") != 42) or
	    (d3.lookup("Chris") != 17) or
	    d2.contains("Chris") or
	    d1.contains("Chris") or
	    d1.contains("Sam")
	    ) {
		cerr << "Huh, that doesn't look right :-(" << endl;
	}

#if TEST_CALL_ALSO
	trace << "call a function with a dictionary parameter" << endl;
	if (containsSamAndChris(d1) or not(containsSamAndChris(d2))) {
	    cerr << "Something strange revealed in TEST_CALL_ALSO test" << endl;
	}
#endif

	trace << "testing toCode,  expect things you could use to initialize d1 and d3" << endl;
	std::cout << "Dictionary d1again = " <<  d1.toCode( ) << ";" << endl;
	std::cout << "Dictionary d3again = " <<  d3.toCode( ) << ";" << endl;
	if ( (d3.toCode() == d1.toCode()) or (d3.toCode() != d3.toCode()) ) {
	    cerr << "Well, that certainly isn't a good sign about 'toCode' :-(" << endl;
	}


	trace << "testing results of toCode, if you edited the code below to use have the output of your toCode" << endl;
	Dictionary d1again = Dictionary();
	Dictionary d3again = Dictionary(Dictionary(Dictionary(), "Sam", 42), "Chris", 17);

   	if ((not d3again.contains("Sam")) or
	    (not d3again.contains("Chris")) or
	    (d3again.lookup("Sam") != 42) or
	    (d3again.lookup("Chris") != 17) or
	    d1again.contains("Chris") or
	    d1again.contains("Sam")
	    ) {
		cerr << "toCode versions of d1 and d3 don't seem quite right" << endl;
	}

	trace << " Several things should be tested that weren't in the starter file test:" << endl;
	trace << " First, if we add another definition of \"Sam\"'s, do we get the right answer when we look up Sam?" << endl;
	Dictionary d4 = Dictionary(d3, "Sam", 7500000);
	if (d4.lookup("Sam") != 7500000 || d3.lookup("Sam") != 42) {
		cerr << "No, not you, Sam, I meant the other Sam!" << endl;
	}

	trace << " Second, it's always good to test numeric things with 0, and string things with \"\", unless they say you can't" << endl;
	Dictionary vacuum = Dictionary(d4, "", 1);
	if (vacuum.lookup("Chris") != 17 || vacuum.lookup("") != 1) {
		cerr << "looks like entering \"\" as a name in the dictionary didn't work out too well" << endl;
	}
	Dictionary none   = Dictionary(d3, "Leslie", 0);
	if (none.lookup("Chris") != 17 || none.lookup("Leslie") != 0) {
		cerr << "looks like entering 0 as a value in the dictionary didn't work out too well" << endl;
	}
	
	trace << "Done with testing dictionary ... if no problems were noted on cerr, Dictionary seems o.k." << endl;
#endif
}
