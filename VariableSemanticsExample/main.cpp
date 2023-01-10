#include <iostream>
#include <vector>

using namespace std;

std::vector<int> biggerStart(std::vector<int> &a, std::vector<int>&b){
    if(a[0]>b[0]){
        return a;
    }
    else{
        return b;
    }
}
std::vector<int> *biggerStartstar(std::vector<int> *a, std::vector<int>*b){
    if((*a)[0]>(*b)[0]){
        return a;
    }
    else{
        return b;
    }
}
int main() {
    std::vector<int>A1={105,210,315};
    std::vector<int>A2={201,202,203};
    std::vector<int>mine=biggerStart(A1,A2);
    mine[2]=mine[2]+20;
    std::vector<int>*minestar=biggerStartstar(&A1,&A2);
    (*minestar)[2]=(*minestar)[2]+10;
    std::vector<int> *p=&mine;
    std::vector<int> &d=mine;
    std::vector<int> *pstar=&mine;
    std::vector<int> &dstar=mine;
    cout<<A1[0]<<" "<<A1[1]<<" "<<A1[2]<<endl;
    cout<<A2[0]<<" "<<A2[1]<<" "<<A2[2]<<endl;
    cout<<"mine"<<" " << mine[0]<<" "<<mine[1]<<" "<<mine[2]<<endl;
    cout<<"minestar" << " " << (*minestar)[0]<<" "<<(*minestar)[1]<<" "<<(*minestar)[2]<<endl;
    cout<<"d"<<" " << d[0]<<" "<<d[1]<<" "<<d[2]<<endl;
    cout<<"p"<<" " << (*p)[0]<<" "<<(*p)[1]<<" "<<(*p)[2]<<endl;
    cout<<"d"<<" " << dstar[0]<<" "<<dstar[1]<<" "<<dstar[2]<<endl;
    cout<<"p"<<" " << (*pstar)[0]<<" "<<(*pstar)[1]<<" "<<(*pstar)[2]<<endl;
/*

    12
    n*j
    (*q)
    i=7;
    n=9;
    int  m2=8;
    i*7
    p*7
    q
    *q=12;
    const int &r=j;
    (*p)*i
    &n
    (*n)
    p=&j;
    *p=3;
    const int m2=j;
    p*i
    &(n*7)
    j=p;
    n=q;
    int &s=m2;
    const int *p2=&m2;
*/
    return 0;
}
