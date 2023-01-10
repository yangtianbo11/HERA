// NOTE --- this is #included from hc_list.h, *** WITHIN NAMESPACE HaverfordCS ***

template <class T> struct list_element {
	T h;
	list<T> t;
	mutable int count;
	list_element(T _h, list<T> _t, int _c) : h(_h), t(_t), count(_c) { }
};


template <class T> list<T>::list()
{
	first=0;
}

template <class T> static void reduce_count(struct list_element<T> *p)
{
	p->count--;
	if (p->count == 0)
		delete p;
}

template <class T> list<T>::~list()
{
	if(first != 0)
		reduce_count(first);
}

template <class T> list<T>::list(const list<T> &init)
{
	first = init.first;
	if (first != 0)
		first->count++;
}

template <class T> list<T> &list<T>::operator=(const list<T> &right)
{
	list<T> &left = *this;

	if (&left == &right)
		return left;
		
	if (left.first != 0)
		reduce_count(left.first);
		
	left.first = right.first;
	if (right.first != 0)
		right.first->count++;
	
	return left;
}


template <class T> bool empty(const list<T> &l)
{
	return l.first == 0;
}
	
template <class T> T head(const list<T> &l)
{
	precondition(! empty(l));
	return l.first->h;
}

template <class T> list<T> rest(const list<T> &l)
{
	precondition(! empty(l));
	return l.first->t;
}

template <class T> list<T>::list(const T &h, const list<T> &r)
{
	first = new list_element<T>(h, r, 1);
}

template <class T> std::ostream &operator<<(std::ostream &o, const list<T> &l)
{
	if (!empty(l))
		o << head(l) << " " << rest(l);
	return o;
}

#if defined ALLOW_SETHEAD
template <class T> void setHead(list<T> &l, const T &val)
{
	precondition(! empty(l));
	l.first->h = val;
}
#endif
