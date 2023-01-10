#include "Tree.h"
#include "TreeHelpers.h"
#include "hc_list_helpers.h" /* list map and reduce, used in one optional version */
#include <algorithm>     /* std::max, as per http://www.cplusplus.com/reference/algorithm/max/ */

using std::string; // use 'string' rather than 'std::string'
using namespace HaverfordCS;


#if TREE_HELPERS_VIA_HOF   /* Only use these if we _are_ using versions written with foldTree */

static int alwaysZero(std::string s){ return 0; }
static int myMax(int oneChildVal, int others){ return std::max(oneChildVal, others); }
static int maxPlusOne(std::string rootVal, int maxKidHeight){ return 1+maxKidHeight; }
int height(Tree aTree)
{
	return foldTree(alwaysZero,  // leaf height is 0
			  myMax,
			  maxPlusOne,
			  aTree);
}

// Instead of naming each trivial function, e.g. alwaysOne for "size"
//  we can use anonymous functions that e.g. return 1 using C++'s [](){} notation
int size(Tree aTree)
{
	return foldTree([](std::string s)->int { return 1; },  // leaf size is 1
			  std::plus<int>(),  // combine children with +
			  [](std::string rootVal, int kidSum)->int { return 1+kidSum; },
			  aTree);
}

int mass(Tree aTree)  // total number of characters
{
	return foldTree([](std::string s)->int { return s.length(); },  // leaf mass is # characters
			  std::plus<int>(),  // combine children with +
			  [](std::string rootVal, int kidSum)->int { return rootVal.length()+kidSum; },
			  aTree);	
}

int shear(Tree aTree) // total number of characters, each weighted by 2**depth
{
	return foldTree([](std::string s)->int { return s.length(); },  // leaf shear=mass= # characters
			  std::plus<int>(),  // combine children with +
			  [](std::string rootVal, int kidSum)->int { return rootVal.length()+2*kidSum; }, // note 2*
			  aTree);	
}


/***** toCode can also be done with a fold/reduce, but I like a slightly different structure *****/

static std::string stringToCode(std::string s) { return "\"" + s + "\""; } // should turn " in s into \", etc., but I'm lazy
string foldToCode(string rootVal, list<string> hedgeCodes)
{
	if (empty(hedgeCodes)) {
		return "Tree("+stringToCode(rootVal)+")";   // could skip base case, but code looks ugly with those empty lists
	} else {
		string kids = 
			foldr<string, string>([ ](string nextTreeCode, string listCode)->string {
					         return "list<Tree>(" + nextTreeCode + ", " + listCode + ")"; },
				              string(""),
				              hedgeCodes);
		return "Tree("+stringToCode(rootVal) + ", " + kids + ")";
	}
}
std::string toCode(Tree aTree)
{
	return foldTreeStr(foldToCode, aTree);
}

std::string toCodeMiniButVerbose(Tree aTree)
{
	return foldTreeStr([ ](string rootVal, list<string> hedgeCodes) {
			      return  "Tree("+stringToCode(rootVal) + ", " +
				      foldr<string,string>([ ](string nextTreeCode, string listCode)->string {
					                      return "list<Tree>(" + nextTreeCode + ", " + listCode + ")"; },
					                   "",
					      	           hedgeCodes) +
				      ")"; },
		           aTree);
}

#endif


// map "func" on tree "aTree":
Tree mapTree(std::function<std::string (std::string)> func, Tree aTree)
{
	// in the anonymous function below, we "capture" func so it can be passed to mapTree
	return Tree(func(aTree.value()),
		    map<Tree,Tree>([func](Tree aChild)->Tree { return mapTree(func, aChild); },
				   aTree.children()));
}

// fold "aTree" by * using processLeaf on leaves (thus producing a list of int's at the bottom level),
//                   * using combineSibs to combine (int) results of reducing a node's children
//                   * using combineKids to combine that (int) combined result of kid reductions, with parent value
// foldTree works best when we don't care about the order and position of children in the list; otherwise, use foldTree
//
// Written via process from "software engineering" discussion here:
//          https://docs.google.com/document/d/1MPrVXs_YSTctVWEt7psNkYSdosJB3dQk1HfIhLbBHA0#heading=h.1rkkpp8myx9
// i.e., take the common elements of height, size, map, shear, etc., and leave variation and parameters, often functions
//
static int foldHedge(std::function<int (std::string)> processLeaf,
		     std::function<int (int oneChildVal, int oneOrMoreChildrenVals)> combineSibs,
		     std::function<int (std::string, int combinedKidValues)> combineKids,
		     list<Tree> aHedge)
{
	precondition(not empty(aHedge));
	if (empty(rest(aHedge))) {
		return              foldTree(processLeaf, combineSibs, combineKids, head(aHedge));
	} else {
		return combineSibs( foldTree(processLeaf, combineSibs, combineKids, head(aHedge)),
				   foldHedge(processLeaf, combineSibs, combineKids, rest(aHedge)));
	}
}
int foldTree(std::function<int (std::string)> processLeaf,
	     std::function<int (int oneChildVal, int oneOrMoreChildrenVals)> combineSibs,
	     std::function<int (std::string, int combinedKidValues)> combineKids,
	     Tree aTree)
{
	if (aTree.isALeaf()) {
		return processLeaf(aTree.value());
	} else {
		return combineKids(aTree.value(), foldHedge(processLeaf, combineSibs, combineKids, aTree.children()));
	}
}
static std::string foldHedgeStr(std::function<std::string (std::string)> processLeaf,
		     std::function<std::string (std::string oneChildVal, std::string oneOrMoreChildrenVals)> combineSibs,
		     std::function<std::string (std::string, std::string combinedKidValues)> combineKids,
		     list<Tree> aHedge)
{
	precondition(not empty(aHedge));
	if (empty(rest(aHedge))) {
		return              foldTreeStr(processLeaf, combineSibs, combineKids, head(aHedge));
	} else {
		return combineSibs( foldTreeStr(processLeaf, combineSibs, combineKids, head(aHedge)),
				   foldHedgeStr(processLeaf, combineSibs, combineKids, rest(aHedge)));
	}
}
std::string foldTreeStr(std::function<std::string (std::string)> processLeaf,
	     std::function<std::string (std::string oneChildVal, std::string oneOrMoreChildrenVals)> combineSibs,
	     std::function<std::string (std::string, std::string combinedKidValues)> combineKids,
	     Tree aTree)
{
	if (aTree.isALeaf()) {
		return processLeaf(aTree.value());
	} else {
		return combineKids(aTree.value(), foldHedgeStr(processLeaf, combineSibs, combineKids, aTree.children()));
	}
}

// fold "aTree" by applying combineOneLevel to the root value and a list of the folded results of kids

int foldTreeStr(std::function<int (std::string rootVal, HaverfordCS::list<int> foldedKids)> combineOneLevel, Tree aTree)
{
	return combineOneLevel(aTree.value(),
			       map<Tree,int>([combineOneLevel](Tree aChild)->int {
					                       return foldTreeStr(combineOneLevel, aChild);},
				             aTree.children()));
}


#if TREE_HELPERS_VIA_HOF   /* Only use these if we _are_ using versions written with foldTree */

int height2(Tree aTree)
{
	return foldTree([](std::string, HaverfordCS::list<int> foldedKids)->int {
				return 1+foldr<int,int>(myMax, -1, foldedKids); },
		           aTree);
}

// this one produces a string, since I happen to want to use it for toCode

string foldTreeStr(std::function<string (std::string rootVal, HaverfordCS::list<string> foldedKids)> combineOneLevel, Tree aTree)
{
	return combineOneLevel(aTree.value(),
			       map<Tree,string>([combineOneLevel](Tree aChild)->string {
					                          return foldTreeStr(combineOneLevel, aChild);},
				                aTree.children()));
}

#endif
