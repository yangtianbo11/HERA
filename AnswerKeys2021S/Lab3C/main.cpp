#include "test-Tree.h"
#include <iostream>
#include "streams.h"

using namespace std;


int main()
{
	trace << "Trace output started, in main function." << endl;
	debug << "Debugging output will be sent to this stream" << endl;
	cout << "Standard output" << endl;
	cerr << "And error output" << endl;
	try {
		if (tree_test()) {
			cout << "tree test looks  good" << endl;
			return 0;
		} else {
			cout << "Rats! Something wrong somewhere in tree test" << endl;
			return 1;
		}
	} catch (...) {
		cerr << "Oops, uncaught exception. Perhaps you took the head of an empty list?" << endl;
		return 2;
	}
}








