#include <iostream>
#include <string>
#include <cmath>
#include "automaton.h"

using namespace std;

int main()
{
	string file="file";
	string input;
	// input="bbaaaa";
	// input="";
	// input="a";
	// input="aba";
	// input="aa";
	input="b";
	cout<<input<<" result:"<<automaton(file,input)<<endl;
	return 1;
}