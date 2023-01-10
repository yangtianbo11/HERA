#include "Dictionary.h"
#include <hc_list.h>
#include <utility>

using std::cout;
//using std::cerr;
//using std::cin;
using std::endl;

const int nonsense=-999;

bool Dictionary::contains(string s){
    list<dictEntry> temp=*repList;
    while(!empty(temp)){
        dictEntry a=first(temp);
        if(s==a.first){
            return true;
        }
        temp=rest(temp);
    }
    return false;
}

int Dictionary::lookup(string s){
    list<dictEntry> temp=*repList;
    while(contains(s)){
        dictEntry a=first(temp);
        if(s==a.first){
            return a.second;
        }
        temp=rest(temp);
    }
    cout<<"string not found in dictionary lookup"<<endl;
    return nonsense;
}

string Dictionary::toCode(){
    return code;
}

bool Dictionary::isEmpty(){
    return empty(*repList);
}

void Dictionary::print() {
    list<dictEntry> temp=*repList;
    while(!empty(temp)){
        dictEntry a=first(temp);
        cout << a.first + ": " + std::to_string(a.second) << endl;
        temp=rest(temp);
    }
    //cout << "Second solution:" << endl;
    //cout << *repList << endl;
}
/* this set function doesn't work
bool Dictionary::set(string s, int n){
    list<dictEntry> temp=*repList;
    while(contains(s)){
        dictEntry a=first(temp);
        if(s==a.first){
            a.second=n;
            return true;
        }
        temp=rest(temp);
    }
    cout<<"string not found in dictionary lookup"<<endl;
    return false;
}*/