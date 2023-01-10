#if 1   // turn this to "#if 1" (or just remove this line and matched endif) to include Dictionary.h
#include "Dictionary.h"    // this had better exist, if we want this to work
#endif
#include "test-Dictionary.h"

#include <iostream>

void testDictionary()
{

	Dictionary d1 = Dictionary(); // an empty dictionary
	// Dictionary d1;  // equivalently

	Dictionary d2 = Dictionary(Dictionary(), "Sam", 42); // required
	// or  Dictionary d2 = Dictionary(d1, "Sam", 42);    // equivalent
	// or, Dictionary d2 = Dictionary().with("Sam", 42); // if you want to write that
	// or, Dictionary d2; d2.insert("Sam", 42); // if doing imperative

    // let's see if "Sam" is home...
	if (d1.contains("Sam")) {
		std::cerr << "Huh, found Sam in an empty dictionary :-(" << std::endl;
		// Wisconsin Tourism Federation needs to know about any problems.
	}
	if (not d2.contains("Sam")) {
		std::cerr << "Huh, Can't find Sam in d2 :-(" << std::endl;
	}

	Dictionary d3 = Dictionary(
				   Dictionary(
					      Dictionary(),
					      "Sam", 42),
				   "Chris", 17);
	// or  Dictionary d3 = Dictionary(d2, "Chris", 17);
	// or, Dictionary d3 = d2; d3.add("Chris", 17); // if doing imperative

	if ((not d3.contains("Sam")) or
	    (not d3.contains("Chris")) or
	    (d3.lookup("Sam") != 42) or
	    (d3.lookup("Chris") != 17) or
	    d2.contains("Chris") or
	    d1.contains("Chris") or
	    d1.contains("Sam")
	    ) {
		std::cerr << "Huh, that doesn't look right :-(" << std::endl;
	}

	// Expect things you could use to initialize d1 and d3
	std::cout << d1.toCode( )<<std::endl;
	std::cout << d3.toCode( )<<std::endl;
	if ( (d3.toCode() == d1.toCode()) or
	     (d3.toCode() != d3.toCode()) ) {
	    std::cerr << "Well, that certainly isn't a good sign about 'toCode' :-(" << std::endl;
	}

#if 1   // turn this to "if 1" to enable this function
	Dictionary d1again = Dictionary();
    Dictionary d3again = Dictionary(Dictionary(Dictionary(), "Sam", 42), "Chris", 17);

   	if ((not d3again.contains("Sam")) or
	    (not d3again.contains("Chris")) or
	    (d3again.lookup("Sam") != 42) or
	    (d3again.lookup("Chris") != 17) or
	    d1again.contains("Chris") or
	    d1again.contains("Sam")
	    ) {
		std::cerr << "toCode versions of d1 and d3 don't seem quite right" << std::endl;
	}

#endif
}
