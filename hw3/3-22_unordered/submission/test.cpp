#include <iostream>
#include <string>
#include <cmath>
#include "automaton.h"

using namespace std;

int main()
{
	string file="file6";
	string input;
	// input="bbaaaa";
	// input="c";
	input="a";
	// input="aba";
	// input="aa";
	// input="b";
	// input="";
	bool output=0;
	try
	{
		output=automaton(file,input);
		cout<<"Input is:"<<input<<endl;
		cout<<"Accepted state:"<<output<<endl;
	}
	catch (string error)
	{
		cout<<"State "<<error<< " is not fully defined for all letters in alphabet in transition funcitons!"<<endl;
	}
	catch (char error)
	{
		cout<<"The letter "<<error<< " doesn't exist in the alphabet."<<endl;
	}
	catch (State_Error MyError)
	{
		cout<<"The state "<<MyError.str<<" doesn't exist in the state list!"<<endl;
	}
	catch (In_Error MyError)
	{
		cout<<"There are more than one initial state (like "<<MyError.str<<")!"<<endl;
	}
	return 1;
}