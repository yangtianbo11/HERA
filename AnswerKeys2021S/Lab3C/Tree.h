#ifndef TREE_H_
#define TREE_H_

#include <string>
#include <hc_list.h>

class Tree {
public:
	explicit Tree(std::string rootVal);
	Tree(std::string rootVal, HaverfordCS::list<Tree> kids);

	bool isALeaf() const;
	std::string value() const;
	HaverfordCS::list<Tree> children() const;
	int nChildren() const;
	Tree child(int i) const;

private:
	std::string _v;
	HaverfordCS::list<Tree> _kids;
};

bool operator==(Tree left, Tree right);

#endif /*TREE_H_*/
