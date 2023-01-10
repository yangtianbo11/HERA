// Put your methods & Functions for trees here, or replace this file with your Tree.cpp
// Optionally, copy other .cpp files you created in the Tree-C++ file intto this project.

#include <string>
#include <hc_list.h>
#include <hc_list_helpers.h>
#include "Tree.h"

using std::string; // use 'string' rather than 'std::string'
using HaverfordCS::list;

#include "streams.h"

// Tree methods/functions go here

Tree::Tree(std::string rootVal) : _v(rootVal)
{
}

Tree::Tree(std::string rootVal, HaverfordCS::list<Tree> kids)
{
    _v = rootVal;
    _kids = kids;
}


bool Tree::isALeaf() const
{
    return empty(_kids);
}

std::string Tree::value() const
{
    return _v;
}

list<Tree> Tree::children() const
{
    return _kids;
}

int Tree::nChildren() const
{
    return length(_kids);
}

template<class T> static T index(list<T> l, int i)
{
    if (empty(l)) {
        trace << "Bother, bad index" << std::endl;
        throw std::string("D'oh; indexed off an empty list");
    } else if (i == 0) {
        return head(l);
    } else {
        return index(rest(l), i-1);
    }
}

Tree Tree::child(int i) const
{
    try {
        return index(_kids, i);
    } catch (string s) {
        std::cerr << "Looks like a bad index: " << s << std::endl;
        throw s + " ... handle this somewhere else, I guess";
    }
}

bool operator==(Tree left, Tree right)
{
    return left.value() == right.value() && left.children() == right.children();
}
