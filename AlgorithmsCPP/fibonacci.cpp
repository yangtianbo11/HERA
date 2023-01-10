//
// Created by yangt on 2/24/2021.
//
#include <logic.h>
#include <iostream>
#include "fibonacci.h"

using namespace std;

long long recursiveFibonacci(int n){
    if(n<=2){
        return 1;
    }
    else{
        return recursiveFibonacci(n-1)+recursiveFibonacci(n-2);
    }
}

long long iterativeFibonacci(int n){
    long long previous=1;
    long long current=1;
    if(n>2) {
        for (int i=2; i<n; ++i) {
            long long temp=current;
            current=current+previous;
            previous=temp;
        }
    }
    return current;
}

long long fibonacci(int i){
    precondition(i>0);
    long long recur=recursiveFibonacci(i);
    long long iter=iterativeFibonacci(i);
    assert(recur==iter);
    return recur;
}
void showTestResults(int i){
    cout<<"fibonacci "<<i<<"="<<fibonacci(i)<<endl;
}

void testFibonacci(){
    int test=9;
    showTestResults(test);
    showTestResults(1);
    showTestResults(2);
    showTestResults(3);
    showTestResults(4);
    showTestResults(44);
    showTestResults(50);
    showTestResults(75);
}