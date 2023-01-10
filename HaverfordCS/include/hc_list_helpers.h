#if ! defined LIST_HELPERS_H
#define LIST_HELPERS_H

#include <hc_list.h>
#include <functional>

namespace HaverfordCS {

// find the length of the list; takes O(length(l)) time
template <class T> int length(const list<T> &il);

// easy ways to build lists of 1-5 items...
template <class T> list<T> ez_list(const T &one);
template <class T> list<T> ez_list(const T &one, const T &two);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine, const T&ten);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine, const T&ten, const T&eleven);
template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine, const T&ten, const T&eleven, const T&twelve);

template <class T> bool operator==(const list<T> &l1, const list<T> &l2);

// find i in list il, answer true (it's there) or false (it's not)
template <class T> bool find(const T &i, const list<T> &il);
// find i in list il, return 0 if at head, 1 if next, etc., or -1 if not there
template <class T>  int index_of(const T &i, const list<T> &il);

// find an element i for which f(i) is true in il, return true iff one is there
// template <class T> bool find(bool (*f)(const T &i), const list<T> &il);
// template <class T> bool find(bool (*f)(T i), const list<T> &il);
template <class T> bool find(std::function<bool (T)> f, const list<T> &il);

// return the thing that was found, as above
// template <class T> const T retrieve(bool (*f)(const T &i), const list<T> &il);
template <class T> const T retrieve(std::function<bool (T)> f, const list<T> &il);

// precondtion: il should have at least one element for which f is true
// postcondition: return that element (or throw "false" as an exception if it's not there)
// 		  if there is more than one such element, return the one closest to the head

// produce a list of those elements i of il for which f(i) is true
// (in the original order)
// template <class T> list<T> filter(bool (*f)(const T &i), const list<T> &il);
template <class T> list<T> filter(std::function<bool (T)> f, const list<T> &il);

// produce a list of the results of applying the function f to each item in il
template <class T1, class T2> list<T2> map(std::function<T2 (T1)> f, const list<T1> &il);

// various approaches to reduce, named for equivalent Haskell functions
template <class T1, class T2>  T2 foldl(std::function<T2 (T2, T1)> f, T2 base,  const list<T1> &l);
template <class T1, class T2>  T2 foldr(std::function<T2 (T1, T2)> f, T2 base,  const list<T1> &l);
template <class T1, class T2> T2 reduce(std::function<T2 (T1, T2)> f, T2 def,   const list<T1> &l);
template <class T1>           T1 foldl1(std::function<T1 (T1, T1)> f,           const list<T1> &l);

#include <hc_list_helpers.t>

}

#endif
