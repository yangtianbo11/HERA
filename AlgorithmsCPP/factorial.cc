// recursive and iterative factorial calculation in C++
//  by Dave Wonnacott, Summer 2005, as an example for CMSC 245

#include <logic.h>
#include <iostream>

#include "factorial.h"

using namespace std;

long long factorial_rec (int n)
{
	precondition(n >= 0);
	if (n > 0) {
		return n * factorial_rec(n-1);
	}
	else {
		return 1;
	}
}

long long factorial_iter (int n)
{
	precondition(n >= 0);
	long long f = 1;
	for (int i=2 ; i<=n ; i++)
		f = f * i;
	return f;
}

long long factorial (int n)
{
	precondition(n >= 0);
	long long recursive_factorial = factorial_rec(n);
	long long iterative_factorial = factorial_iter(n);
	assert(recursive_factorial == iterative_factorial);
	return recursive_factorial;
}

void doTestFactorial(int i)
{
    cout << i << "! = " << factorial(i) << endl;
}

void testFactorial()
{
    int i = 42;
    double d = 1.0;

    cout << endl << "============ test Factorial ============" << endl << endl;

    doTestFactorial(0);
    doTestFactorial((int) d);
    doTestFactorial(2);
    doTestFactorial(6);
    doTestFactorial(10);
    doTestFactorial(i);
    doTestFactorial(150);
    cout << endl << "  Note: arithmetic on large integers can reveal C++'s expectation" << endl;
    cout <<         "        that you think about the possible range of integer values" << endl;
    cout <<         "        and the implications of operators working only in this range." << endl;
}

