/*
 * Dictionary.h
 *
 *  Created on: Nov 18, 2019
 *      Author: davew
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <hc_list.h>

class Dictionary {
public:
	Dictionary();  // empty dictionary
	Dictionary(Dictionary d, std::string key, int value);

	bool contains(std::string key) const;
	int  lookup(std::string key) const;     // throws an error if a name isn't there

	std::string toCode() const;

private:
	struct entry {  // equivalently, "class entry { public: ..."
		std::string k;
		int v;
	};
	HaverfordCS::list<entry> entries;

	// static methods are shared by all objects
	static bool containsHelper(HaverfordCS::list<entry> l, std::string key);
	static int  lookupHelper(HaverfordCS::list<entry> l, std::string key);
	static std::string toCodeHelper(HaverfordCS::list<entry> l);
};


#endif /* DICTIONARY_H_ */
