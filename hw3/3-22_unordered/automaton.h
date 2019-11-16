#ifndef AUTOMATON_H
#define AUTOMATON_H
#include <string>
using namespace std;
class State_Error
{
public:
	string str;
	State_Error(string input)
	{
		str=input;
	}
};
class In_Error
{
public:
	string str;
	In_Error(string input)
	{
		str=input;
	}
};
bool automaton(string file, string input);
#endif

