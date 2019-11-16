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
	bool output=0;
	try
	{
		output=automaton(file,input);
		cout<<input<<" result:"<<output<<endl;
	}
	catch (int error)
	{
		char letter=error%256;
		if (error/256<256)
		{
			char state=error/256;
			cout<<"State "<<state<<" not defined for letter "<<letter<<"!"<<endl;
		}
		else

		{
			char state2=(error/256)%256;
			char state1=(error/256/256)%256;
			cout<<"State "<<state1<<state2<<" not defined for letter "<<letter<<"!"<<endl;
		}
		
	}
	return 1;
}