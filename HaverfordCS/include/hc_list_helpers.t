template <class T> int length(const list<T> &il)
{
	if (empty(il))
		return 0;
	else return 1+length(rest(il));
}


template <class T> list<T> ez_list(const T &one)
{
	return list<T>(one, list<T>());
}

template <class T> list<T> ez_list(const T &one, const T &two)
{
	return list<T>(one, list<T>(two, list<T>()));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three)
{
	return list<T>(one, list<T>(two, list<T>(three, list<T>())));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four)
{
	return list<T>(one, list<T>(two, list<T>(three, list<T>(four, list<T>()))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five)
{
	return list<T>(one, list<T>(two, list<T>(three, list<T>(four, list<T>(five, list<T>())))));
}


template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six, list<T>()))))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six,
							       list<T>(seven, list<T>())))))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six,
							       list<T>(seven,
								       list<T>(eight, list<T>()))))))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six,
							       list<T>(seven,
								       list<T>(eight,
									       list<T>(nine, list<T>())))))))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine, const T&ten)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six,
							       list<T>(seven,
								       list<T>(eight,
									       list<T>(nine,
										       list<T>(ten, list<T>()))))))))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine, const T&ten, const T&eleven)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six,
							       list<T>(seven,
								       list<T>(eight,
									       list<T>(nine,
										       list<T>(ten,
											       list<T>(eleven, list<T>())))))))))));
}

template <class T> list<T> ez_list(const T &one, const T &two, const T &three, const T &four, const T&five, const T&six, const T&seven, const T&eight, const T&nine, const T&ten, const T&eleven, const T&twelve)
{
	return list<T>(one,
		       list<T>(two,
			       list<T>(three,
				       list<T>(four,
					       list<T>(five,
						       list<T>(six,
							       list<T>(seven,
								       list<T>(eight,
									       list<T>(nine,
										       list<T>(ten,
											       list<T>(eleven,
												       list<T>(twelve, list<T>()))))))))))));
}


template <class T> bool operator==(const list<T> &l1, const list<T> &l2)
{
	if (empty(l1)) {
		return empty(l2);
	} else {
		return !empty(l2) && (head(l1) == head(l2)) && (rest(l1) == rest(l2));
	}
}


template <class T> bool find(const T &i, const list<T> &il)
{
	if (empty(il))
		return false;
	else if (i == head(il))
		return true;
	else
		return find(i, rest(il));
}
template <class T> int index_of(const T &i, const list<T> &il)
{
	if (empty(il))
		return -1;
	else if (i == head(il))
		return 0;
	else {
		int where = index_of(i, rest(il));
		return where < 0 ? where : where+1;
	}
}

template <class T> bool find(std::function<bool (T)> f, const list<T> &il)
{
	if (empty(il))
		return false;
	else if (f(head(il)))
		return true;
	else
		return find(f, rest(il));
}

template <class T> const T retrieve(std::function<bool (T)> f, const list<T> &il)
{
	if (empty(il))
		throw false;
	else if (f(head(il)))
		return head(il);
	else
		return retrieve(f, rest(il));
}

template <class T> list<T> filter(std::function<bool (T)> f, const list<T> &il)
{
	if (empty(il))
		return il;
	else if (f(head(il)))
		return list<T>(head(il), filter(f, rest(il)));
	else
		return filter(f, rest(il));
}


template <class T1, class T2> list<T2> map(std::function<T2 (      T1  )> f, const list<T1> &il)
{
	if (empty(il))
		return list<T2>();
	else
		return list<T2>(f(head(il)), map<T1, T2>(f, rest(il)));
}

// from Hoogle:
// foldl  :: (a -> b -> a) -> a -> [b] -> a  // b is T1, the type of the list, below
// foldl1 :: (a -> a -> a) ->      [a] -> a
//  and thanks to https://wiki.haskell.org/Fold, which I used to check my algorithm
template <class T1, class T2> T2 foldl(std::function<T2 (T2, T1)> f, T2 base, const list<T1> &l)
{
	if (empty(l)) {
		return base;
	} else {
		return foldl<T1, T2>(f, f(base, head(l)), rest(l));
	}
}
template <class T1>          T1 foldl1(std::function<T1 (T1, T1)> f,          const list<T1> &l)
{
	return foldl<T1, T1>(f, head(l), rest(l));
}


// from Hoogle:
// foldr  :: (a -> b -> b) -> b -> [a] -> b  // a is T1, the type of the list, below
// foldr1 :: (a -> a -> a)      -> [a] -> a
//  and thanks to https://wiki.haskell.org/Fold, which reminded me of the clean way to do this
template <class T1, class T2> T2 foldr(std::function<T2 (T1, T2)> f, T2 base, const list<T1> &l)
{
	if (empty(l)) {
		return base;
	} else {
		return f(head(l), foldr<T1, T2>(f, base, rest(l)));
	}
}

// THIS SHOULD BE ONLY ONE TYPE, FOR HEAD(L) TO BE SAME TYPE AS DEF SAME AS REDUCE!
template <class T1, class T2> T2 reduce(std::function<T2 (T1, T2)> f, T2 def, const list<T1> &l)
{
	if (empty(l))
	   return def;
	else if (empty(rest(l)))
	   return head(l);
	else return foldl<T1, T2>(f, head(l), rest(l));  // foldl1 but with T1 and T2
}

#if 0

// TODO:
this is in hc_list, not hc_list helpers ... maybe move it for 2016-17?
// No, printing is too vital to students figuring out what's going on; keep that easy

template <class T> std::ostream &operator<<(std::ostream &o, const list<T> &l)
{
	if (empty (l))
		return o << "list()";
	else {
		o << "list(" << head(l);
		map([&o](T e) { o << ", " << e; return; }, rest(l));
		return o << ")";
	}
}
#endif
