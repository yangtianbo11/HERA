/*
 * Dictionary.cc
 *      Author: davew
 */

#include "Dictionary.h"
#include <hc_list_helpers.h>
using HaverfordCS::list;


Dictionary::Dictionary()
{
	entries = list<entry>();
}

Dictionary::Dictionary(Dictionary old, std::string key, int value)
{
	entry e;
	e.k = key;
	e.v = value;
	entries = list<entry>(e, old.entries);
}

bool Dictionary::containsHelper(list<entry> l, std::string key)  // static
{
	if (empty(l))
		return false;
	else
		return key==head(l).k || containsHelper(rest(l), key);
}
		
bool Dictionary::contains(std::string key) const
{
	return containsHelper(entries, key);
}

int Dictionary::lookupHelper(list<entry> l, std::string key)  // static
{
	if (empty(l))
		throw "Oops, did a lookup on something not in the dictionary:" + key;
	else if (key==head(l).k)
		return head(l).v;
	else
		return lookupHelper(rest(l), key);
}

int Dictionary::lookup(std::string key) const
{
	return lookupHelper(entries, key);
}


std::string Dictionary::toCodeHelper(HaverfordCS::list<entry> l)
{
	if (empty(l)) {
		return "Dictionary()";
	} else {
		return "Dictionary(" + toCodeHelper(rest(l)) + ", \"" + head(l).k + "\", " + std::to_string(head(l).v) + ")";
	}
}

std::string Dictionary::toCode() const
{
	return toCodeHelper(entries);
}
