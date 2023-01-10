#ifndef EVAL_H_
#define EVAL_H_


#include "Tree.h"

#if defined TREE_H_
int eval(Tree expression); // find value, e.g. for Tree("*", ez_list(Tree("6"), Tree("7"))), get the integer 42
#else
int eval(bool parseResult); // what we can do without a Tree class
#endif

#endif /*EVAL_H_*/
