//
// Example C++ program for CS245 Lab 1C - Last updated 2021
//
// Compile in Eclipse, or via "g++245" or "clang245" (in the QuaCS lab),
//         or via clang -std=c++17 -I/home/courses/include *.cc -L/home/courses/lib -lcourses -lstdc++ -o Algorithms-C++  # or g++ instead of clang
//	   after checking via clang-tidy -checks=cppcoreguidelines-* -header-filter=.* *.cc -- -std=c++17 -I/home/courses/include 
//

#include <iostream>

#include "factorial.h"
#include "stringAnd_HOF_Examples.h"
#include "fibonacci.h"

using namespace std;


int main()
{
    cout << endl << addExclamation("Welcome to the C++ Example program") << endl;

//    testFactorial();
//    testStringFunctions();
    testHigherOrderStringFunction();
//    testFibonacci();

    cout << endl;
    return 0;  // return 0 means the program succeeded
}
