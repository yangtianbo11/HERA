#include <functional>   // provides std::function
#include <string>	// std::string
#include <iostream>	// cout, endl, etc...

#include <logic.h>	// Haverford's logic.h, for precondition, assert, etc.
//#include <algorithm> //this line came from https://stackoverflow.com/questions/3867890/count-character-occurrences-in-a-string-in-c
#include "stringAnd_HOF_Examples.h"  // note that this .h includes <string> for us

using namespace std;	// This lets us write 'string' instead of 'std::string', etc.


// a simple example of a function that takes a string parameter and returns a string
string addExclamation(string sentence)
{
	if (sentence[sentence.length()-1] == '.') {
		return sentence.substr(sentence.length()-1, 1) + "!";
	}
	else {
		return sentence + "!";
	}
}


// a simple example that takes a string parameter and returns an int
//https://stackoverflow.com/questions/3867890/count-character-occurrences-in-a-string-in-c
int countXs(string s){
	int n_Xs = 0;
	for (char c : s) {
		if (c == 'X') n_Xs++;
	}
	// Old-style C equivalent still works, too:
	int n_XsAgain = 0;
	unsigned int i; // the one above could be 'unsigned' too
	for (i = 0; i<s.length(); i++) { // 'i' takes on each valid index
		if (s[i] == 'X') n_XsAgain++;
	}
	assert(n_Xs == n_XsAgain); // let me know, if this isn't true
	return n_Xs;

//    size_t n = std::count(s.begin(), s.end(), 'x'); //this line came from https://stackoverflow.com/questions/3867890/count-character-occurrences-in-a-string-in-c
/* a. An hypothetical example of copyright violation but not academic dishonesty would be to copy work
 * from a website to use in your class project and citing the source. The original creator of the work
 * can still sue you for copyright infringement for using his/her work, but your instructor will not
 * penalize you for academic dishonesty because you let it be known that it is not you work.
 * b. An hypothetical example of academic dishonesty but not copyright violation is if you used an idea
 * from another person in your project that is not/cannot be protected under copyright law and did not
 * cite the source. For example, collaborating with a peer without letting the instructor know and
 * without acknowledging the peer's contribution constitutes as academic dishonesty but not copyright
 * infringement as it doesn't protect "help on a project". Another example of academic dishonesty that
 * is not a copyright violation is copying work from the public domain without citations.*/
}

string repeatString(string s){
    string chant="";
    for(int i=0; i<s.length(); i++){
        chant=chant+s;
    }
    return chant;
}

void testStringFunctions()
{
	cout << endl << "============ test string functions and built-ins ============" << endl << endl;

	cout << "addExclamation(\"My string function works\") = " <<
			 addExclamation("My string function works") << endl;

	string sixX = "This boX of siX strings actually only has two X's in it ... no, wait, it has three X's now; I mean four X's; five X's! NEVERMIND!!";
	cout << "countXs(\"" << sixX << "\") = " << countXs(sixX) << endl;
	cout << endl << "  Note: string operations can sometimes reveal C++'s expectation" << endl;
	cout <<         "        that you think about the possible range of integer values" << endl;
	cout <<         "        and the implications of operators working only in this range:" << endl;

	cout << "string length test ... what is string(\"test\").length()?   " << string("test").length() << endl;
	cout << "string length test ... what is string(\"test\").length()-5? " << string("test").length()-5 << endl;

	cout<<"testing repeatString"<<endl;
	cout<<"ow turns into: "<<repeatString("ow")<<endl;
	cout<<"wow turns into: "<<repeatString("wow")<<endl;
	cout<<"Dave turns into: "<<repeatString("Dave")<<endl;
	cout<<"hello turns into: "<<repeatString("hello")<<endl;
	cout<<"hello world turns into: "<<repeatString("hello world")<<endl;
}


// compute f(f(s)), for whatever value of s
string twice_f(string s, std::function<string (string)> f)
{
    return f(f(s));
}

string callAndConcatenate(string a, string b, std::function<string (string)> f){
    return f(a)+f(b);
}

void testHigherOrderStringFunction()
{
	cout << endl << "============ test string functions and built-ins ============" << endl << endl;
	cout << "twice_f(\"Test\", addExclamation) = " <<
			 twice_f("Test", addExclamation) << endl;

	cout<<"callAndConcatenate test"<<endl;
	cout<<"exclaim wow cow!: "<<callAndConcatenate("wow", "cow", addExclamation)<<endl;
    cout<<"repeat CS rocks : "<<callAndConcatenate("CS", "rocks", repeatString)<<endl;
}