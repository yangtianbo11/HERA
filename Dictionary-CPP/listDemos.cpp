/*
 * list_demo.cc --- simple examples of lists using HaverfordCS::list and std::list
 */

#include <hc_list.h>	// gets files from /home/courses/include folder, thanks to -I flag on compiler
#include <hc_list_helpers.h>
#include <list> 	// gets C++ standard library files, thanks to C++ standard
#include <iostream>

#include "listDemos.h"

using std::cout;
using std::endl;
#include "streams.h"  /* for "trace" output */


// Demo of basic recursive algorithm design lining up with basic recursive data structure design:
//  the algorithm checks for the base case of the type (e.g., an empty list)
//  and calls itself recursively for the recursive part of the type (e.g., the rest of the list)

int sumOfSquares(HaverfordCS::list<int> aList)
{
	if (empty(aList)) {
		return 0;
	} else {
		return first(aList)*first(aList) + sumOfSquares(rest(aList));
	}
}
void hcDemoBasicRecursion()
{
	using HaverfordCS::list;      // HC lists are designed to be similar to Scheme lists
	using HaverfordCS::ez_list;

	list<int> example = ez_list(3, 5, 7, 11);	// ez_list is like Scheme's 'list'
	example = list<int>(2, example);	// list is like Scheme's cons

	trace << example << endl;

	trace << "sum of squares is: " << sumOfSquares(example) << endl; 
}
void hcDemoLoops()
{
	using HaverfordCS::list;
	using HaverfordCS::ez_list;

	list<int> example = ez_list(3, 5, 7, 11);
	example = list<int>(2, example);

	trace << example << endl;

	list<int> theList = example;  // we'll change 'theList' but not 'example'
	int total = 0;
	while (!empty(theList)) {
		total = total + first(theList)*first(theList);
		theList = rest(theList);
	}
	trace << "sum of squares is: " << total << endl;
}

int squareMe(const int i) { return i*i; }	// these will be used later
int sum(const int &i, const int &sumSoFar) { return i+sumSoFar; }

static void hcDemoHigherOrder()
{
	using HaverfordCS::list;
	using HaverfordCS::ez_list;

	list<int> example = ez_list(3, 5, 7, 11);
	example = list<int>(2, example);

	trace << example << endl;

	list<int> squares = HaverfordCS::map<int, int>(squareMe, example);	// some compilers need the type of 'map'
	int total = HaverfordCS::reduce<int, int>(sum, 0, squares);

	// alternate notation: don't pre-define the funcion above, but use [ ] to define it inline
	//	list<int> squares = HaverfordCS::map([](int i) { return i*i; }, example);
	//	int total = HaverfordCS::reduce([](int i, int sumSoFar) { return i+sumSoFar; }, 0, squares);
	// or, without even temporary variable "squares":
	// int total = HaverfordCS::reduce([](int i, int sumSoFar) { return i+sumSoFar; },
	//                                 0,
	//                                 HaverfordCS::map([](int i) { return i*i; }, example));
	trace << "sum of squares is: " << total << endl;
}

void hcListDemos()	// Standard Library lists focus more on iteration and imperative style
{
	trace << endl << "Welcome to the three demo functions of HaverfordCS::list" << endl;
	hcDemoBasicRecursion();
	hcDemoLoops();
	hcDemoHigherOrder();
	trace << endl;
}

// list<int> x;  // should error, since there's no 'using' for list in this scope

void stdListDemo()	// Standard Library lists focus more on iteration and imperative style
{
	trace << endl << "Welcome to the demo function of std::list" << endl;
	using std::list;  // FOR SPRING 2021, PLEASE DO NOT USE THIS IN LAB 2C (but, it's o.k. to read it)

	// I wrote the examples below after looking at
	// http://www.cplusplus.com/reference/list/list/list/
	// and http://www.cplusplus.com/reference/iterator/
	// and http://www.cplusplus.com/reference/list/list/insert/
	// and getting help from Nicky Rhodes (a CS245 student):
	list<int> example = list<int>( {3, 5, 7, 11} );
	example.push_front(2);	// Note this is a _mutator_, not a constructor: example is changed

	// this seems not to work: trace << example << endl;
	// so, let's use an iterator to look at each one:
	for (list<int>::iterator i = example.begin(); i != example.end(); i++) {
		trace << *i << " ";
	}
	trace << endl;

	// Note that the standard library also has something named "map",
	//  but that is a _data_type_ similar to a Python dictionary,
	//  not a function or a substitute for Scheme's "map".
	// So, I'll just iterate here and build a new list.

	list<int> squares;
	for (list<int>::iterator i = example.begin(); i != example.end(); i++) {
		int nextSquare = *i * *i;	// * used in two ways; the middle is multiply
		squares.push_back(nextSquare);
	}

	// We'll sum up those squares with a separate loop, though they could be done together,
	//  because I want to demonstrate a shorthand form of iterators:
	int total = 0;
	for (auto eachSquare : squares) {
		total += eachSquare;
	}
	
	trace << "sum of squares is: " << total << endl;
}
