#if ! defined TREE_HELPERS__H
#define TREE_HELPERS__H 1

#include "Tree.h"
#include <functional>

// KEY:
// set this CPP variable to 1 to have size, height, etc., defined in terms of tree folds (set to 0 otherwise)
#if ! defined TREE_HELPERS_VIA_HOF
#define TREE_HELPERS_VIA_HOF 0
#endif

int height(Tree);
int size(Tree);
int mass(Tree);   // total number of characters in the tree, just for fun
std::string heightWithStars(Tree);
std::string longPath(Tree);


std::string toCode(Tree);

// map "func" on tree "aTree":
Tree mapTree(std::function<std::string (std::string)> func, Tree aTree);

// fold "aTree" by * using processLeaf on leaves (thus producing a list of int's at the bottom level),
//                   * using combineSibs to combine (int) results of reducing a node's children
//                   * using combineKids to combine that (int) combined result of kid reductions, with parent value
// this version of foldTree works best when we don't care about the order and position of children in the list
int foldTree(std::function<int (std::string)> processLeaf,
	     std::function<int (int oneChildVal, int oneOrMoreChildrenVals)> combineSibs,
	     std::function<int (std::string, int combinedKidValues)> combineKids,
	     Tree aTree);

// fold "aTree" by applying combineOneLevel to the root value and a list of the folded results of kids
int foldTree(std::function<int (std::string rootVal, HaverfordCS::list<int> foldedKids)> combineOneLevel, Tree aTree);

std::string foldTreeStr(std::function<std::string (std::string rootVal, HaverfordCS::list<std::string> foldedKids)> combineOneLevel, Tree aTree);
std::string foldTreeStr(std::function<std::string (std::string)> processLeaf,
	     std::function<std::string (std::string oneChildVal, std::string oneOrMoreChildrenVals)> combineSibs,
	     std::function<std::string (std::string, std::string combinedKidValues)> combineKids,
	     Tree aTree);

int height2(Tree); // demonstrates second foldTree rather than first

#endif
