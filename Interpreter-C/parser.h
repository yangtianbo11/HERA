#ifndef PARSER_H_
#define PARSER_H_


#include "Tree.h"

// The typedef below makes the name "translatedResult"
//  mean a "Tree" object; this is used so that we
//  can use the same pattern-matching parser for
//  different results, if we want to.
#if defined TREE_H_
typedef Tree ParserResult;
#else
typedef bool ParserResult;
#endif


// ParserResult is a Tree that has:
//  leaves with strings that contain numerals or #t or #f, for literal values as strings of numerals
//  internal nodes with operations and (currently) two children, e.g. for + or <=
//  internal nodes with " call " (note spaces) and a single leaf child that's the name of a function to call
//    (in principle, that could have a list of children, but it currently does not, since we only have "getint" and "exit")

ParserResult matchStartSymbolAndEOF();

#endif /*PARSER_H_*/
