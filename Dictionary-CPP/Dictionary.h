#ifndef DICTIONARY_C_DICTIONARY_H
#define DICTIONARY_C_DICTIONARY_H

#include <hc_list.h>
#include <utility>
//#include <string>

using HaverfordCS::list;
using std::string;

typedef std::pair<string,int> dictEntry;

class Dictionary {
    private: list<dictEntry> *repList;
    string code;

public:
    Dictionary(){
        repList=new list<dictEntry>();
        code="Dictionary()";
    }

    Dictionary(const Dictionary &d, string s, int n){
        dictEntry newPair = dictEntry(s,n);
        this->repList = new list<dictEntry>(newPair, *(d.repList));
        code="Dictionary(" + d.code + ", \"" + s + "\", " + std::to_string(n) + ")";
    }

    bool contains(string s);

    int lookup(string s);

    string toCode();

    bool isEmpty();

    void print();

    //bool set(string s, int n);
};


#endif //DICTIONARY_C_DICTIONARY_H
