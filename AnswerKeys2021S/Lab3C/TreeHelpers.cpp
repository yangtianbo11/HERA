#include "Tree.h"
#include "TreeHelpers.h"
#include "hc_list_helpers.h" /* list map and reduce, used in one optional version */
#include <algorithm>     /* std::max, as per http://www.cplusplus.com/reference/algorithm/max/ */

#if ! TREE_HELPERS_VIA_HOF   /* Only use these if _not_ using versions written with foldTree */

using std::string; // use 'string' rather than 'std::string'
using namespace HaverfordCS;

static int heightMax(list<Tree> aHedge)
{
	if (empty(aHedge)) {
		return -1;  // less than any actual height, and when we add 1 to it to get a leaf's height, it works
	} else {
		return std::max<int>(height(head(aHedge)), heightMax(rest(aHedge)));
	}
}
int heightBRD(Tree aTree)
{
	return 1+heightMax(aTree.children());  // note height of empty hedge is -1, height of hedge of leaves is 0
}
// Alternate approach, for anyone comfortable with list reduce
int myMaxCantUseSTD(int a, int b) { return a>=b ? a : b; }  /* Don't know why I can't get std::max to work here */
int height(Tree aTree)
{
//	return 1+reduce<int,int>(std::max<int>,   -1, map<Tree,int>(height, aTree.children()));
	return 1+reduce<int,int>(myMaxCantUseSTD, -1, map<Tree,int>(height, aTree.children()));
}

string longerString(string soFar, string kidHt)
{
	return soFar.length() > kidHt.length() ? soFar : kidHt;
}

string heightWithStars(Tree aTree)
{
	if (aTree.isALeaf()) {
		return "";
	} else {
		return "*"+foldl<string,string>(longerString,
						"",
						map<Tree,string>(heightWithStars, aTree.children()));
	}
}

string longPath(Tree aTree)
{
	return aTree.value()+foldl<string,string>(longerString,
						  "",
						  map<Tree,string>(longPath, aTree.children()));
}

static int sizeTotal(list<Tree> aHedge)
{
	if (empty(aHedge)) {
		return 0;
	} else {
		return (size(head(aHedge)) + sizeTotal(rest(aHedge)));
	}
}
int size(Tree aTree)
{
	return 1+sizeTotal(aTree.children());
}

static int massTotal(list<Tree> aHedge)
{
	if (empty(aHedge)) {
		return 0;
	} else {
		return mass(head(aHedge)) + massTotal(rest(aHedge));
	}
}
int mass(Tree aTree)  // total number of characters
{
	return aTree.value().length() + massTotal(aTree.children());
}

static std::string hedgeToCode(list<Tree> aHedge)
{
	if (empty(aHedge)) {
		return "list<Tree>()";
	} else {
		return "list<Tree>(" + toCode(head(aHedge)) + ", " + hedgeToCode(rest(aHedge)) + ")";
	}
}
static std::string stringToCode(std::string s) { return "\"" + s + "\""; } // should turn " in s into \", etc., but I'm lazy
std::string toCode(Tree aTree)
{
	if (aTree.isALeaf()) { // could use general case, but it looks awful when printed
		return "Tree(" + stringToCode(aTree.value()) + ")";
	} else {
		return "Tree(" + stringToCode(aTree.value()) + ", " + hedgeToCode(aTree.children()) + ")";
	}
}

#endif
