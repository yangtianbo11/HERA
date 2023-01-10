#include <iostream>
#include <hc_list.h>
#include <hc_list_helpers.h>
#include "test-Tree.h"
#include "Tree.h"
#include "TreeHelpers.h"


using std::cout; using std::endl;
using std::string;
using HaverfordCS::list; using HaverfordCS::ez_list;

#include "streams.h"
#if defined DONT_TRACE_TREE_TEST
// shadow global trace with a file-local static variable of the same name
#include <fstream>  /* needed for ofstream below */
static std::ofstream trace("/dev/null"); // output to "null", which discards all data
#endif


bool tree_test()
{
	trace << "Creating some trees in test-tree.cc" << endl;

	// don't know if the class will use "empty" or "leaf" as the base-case for tree,
	// so create an empty list of children so as to make leaves with the general ctor.
	list<Tree> emptyHedge = list<Tree>();
	
	Tree lt1a = Tree("leaf 1a", emptyHedge);  // this should make a leaf, as should Tree("leaf 1a")
	Tree lt1b = Tree("leaf 1b", emptyHedge);
	Tree mid1 = Tree("mid-node 1", ez_list<Tree>(lt1a, lt1b));
	Tree lt2a = Tree("leaf 2a", emptyHedge);
	Tree lt2b = Tree("leaf 2b ;-)", emptyHedge);  //  Updated test to have ;-), to make lengths different
	Tree lt2c = Tree("leaf 2c", emptyHedge);
	Tree mid2 = Tree("mid-node 2", ez_list<Tree>(lt2a, lt2b, lt2c));
	Tree top  = Tree("top node",   ez_list<Tree>(mid1, mid2));

	trace << "values of lt1a, mid2, and top are: \"" <<
		lt1a.value() << "\", \"" <<
		mid2.value() << "\", and \"" <<
		top.value() << "\"." << endl;

	trace << "is lt1a the same as lt1b? it shouldn't be: " <<
		((lt1a == lt1b) ? "Rats, it is :-(" : "Phew, it's not.") << endl;
	trace << "is lt1a the same as itself? it should be: " <<
		((lt1a == lt1a) ? "Phew, it is." : "Good grief, it's not =:-O") << endl;
	trace << "is mid2 the same as an imposter with different children? it shouldn't be: " <<
		((mid2 == Tree("mid-node 2", ez_list<Tree>(lt1a, lt1b))) ? "Rats, it is :-(" : "Phew, it's not") << endl;

// Add a test creating Sedgewick's tree here

	trace << "Here's C++ code for a tree; it should be possible to copy-paste it back into test-Tree.cc\n" << endl;
// un-comment the next line, and delete the line after that, after writing "toCode"
	cout << toCode(top) << endl;
	trace << "*** Update the test to print a tree once you've written toCode ***" << endl;

	Tree topAgain = top;
	trace << "This should look just like that:";
	trace << toCode(topAgain) << endl;

	trace << "Here it is, excited! " << toCode(mapTree([](string s){return s+"!";}, top)) << endl;

	// NOTE: I didn't run these before sending out the files;
	//       if it looks like there are errors in the test,
	//       please post to Piazza.
	return (
			(trace << "basic checking root value function" << endl, true) &&
			lt1a.value() ==  "leaf 1a" &&
			top.value()  ==  "top node" &&
// un-comment the next lines after adding getChildI()
			(trace << "basic checking child function" << endl, true) &&
			top.child(0).value() == "mid-node 1" &&
// un-comment the next lines after adding size
			(trace << "basic checking size function" << endl, true) &&
			size(mid2) == 4 &&
// un-comment the next line after adding height
			(trace << "basic checking heightWithStars function" << endl, true) &&
			heightWithStars(mid2) == "*" &&
// un-comment the next two after writing operator==
			(trace << "basic checking == operation" << endl, true) &&
			top == top && 
			!(lt2b == lt2c) && 
// un-comment the next one after writing toCode and operator==
			(trace << "basic checking of topAgain via == operation" << endl, true) &&
			top == topAgain &&
			(trace << endl << " *** Now, my own, much-more-interesting, test suite: ***" << endl, true) &&
// =============================================================================
// =============================================================================
// More tests will be used in the grading, so you might want to add some here...
// For example, can you compute "size" by using "map" to create a tree of ".",
//  i.e., with the function [](string s){return ".";}, and then do a reduce
//  that does string concatenation, to produce one long "....." string,
//  and then find the length of that string?
// =============================================================================
// =============================================================================
			(trace << "basic checking longPath function " << longPath(mid2) << endl, true) &&
			longPath(mid2) == "mid-node 2leaf 2b ;-)" &&
			(trace << "checking mapTree" << endl, true) &&
			(mapTree([](string s){return s+"!";}, mid2) ==
			 Tree("mid-node 2!", list<Tree>(Tree("leaf 2a!"),
					     list<Tree>(Tree("leaf 2b ;-)!"),
					     list<Tree>(Tree("leaf 2c!"), list<Tree>()))))) &&
			(trace << "checking less-crazy version of fold" << endl, true) &&
			(foldTreeStr([](string s){return "\"" + s + "\"";}, // leaf
				     [](string one, string rest){return one + ", " + rest;}, // siblings
				     [](string p, string kids){return "\"" + p + "\"(" + kids + ")";},
				     mid2) ==
		         "\"mid-node 2\"(\"leaf 2a\", \"leaf 2b ;-)\", \"leaf 2c\")") &&
//			(trace << "What's the frequency, Kenneth?" << endl, true) &&
			true);  // true here lets me add more tests with && on each one
}
