#include <iostream>
#include <cmath>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include "automaton.h"

using namespace std;


bool automaton(string file, string input)
{
	ifstream myfile;
	string str;
	myfile.open(file);

	unordered_map<char, int> Al;
	unordered_map<string, int> St, Fi;
	string In;
	unordered_map<string,unordered_map<char,string> > Tr;

	getline(myfile,str);
	
	while(getline(myfile,str) && str[0]!='-')
	{
		istringstream mystream(str);
		char temp;
		while(mystream>>temp)
			Al[temp]++;
		str.clear();
	}

	while(getline(myfile,str) && str[0]!='-')
	{
		istringstream mystream(str);
		string temp;
		while(mystream>>temp)
			St[temp]++;
		str.clear();		
	}

	while(getline(myfile,str) && str[0]!='-')
	{
		istringstream mystream(str);
		string temp;
		mystream>>temp;
		In=temp;
		if (!St[In])
		{
			State_Error MyError(temp);
			throw MyError;
		}
		if (mystream>>temp)
		{
			In_Error MyError(temp);
			throw MyError;
		}
		str.clear();
	}

	while(getline(myfile,str) && str[0]!='-')
	{
		istringstream mystream(str);
		string temp;
		while(mystream>>temp)
		{
			if (!St[temp])
			{
				State_Error MyError(temp);
				throw MyError;
			}
			Fi[temp]++;
		}
		str.clear();		
	}

	while(getline(myfile,str) && str[0]!='-')
	{
		istringstream mystream(str);
		string Trash;
		string temp1;
		char temp2;
		string temp3;
		mystream>>temp1;
		if (!St[temp1])
		{
			State_Error MyError(temp1);
			throw MyError;
		}
		mystream>>Trash;
		mystream>>temp2;
		if (!Al[temp2])
			throw temp2;
		mystream>>Trash;
		mystream>>temp3;
		if (!St[temp3])
		{
			State_Error MyError(temp3);
			throw MyError;
		}
		Tr[temp1][temp2]=temp3;
		str.clear();	
	}

	for (auto temp1: St)
	{
		for (auto temp2:Al)
		{
			if (Tr.find(temp1.first)==Tr.end() || Tr[temp1.first].find(temp2.first)==Tr[temp1.first].end())
				throw temp1.first;
		}
	}



	// for (auto temp:Al)
	// {
	// 	cout<<temp.first<<" ";
	// }
	// cout<<endl;

	// for (auto temp: St)
	// {
	// 	cout<<temp.first<<" ";
	// }
	// cout<<endl;

	// for (auto temp: Fi)
	// {
	// 	cout<<temp.first<<" ";
	// }
	// cout<<endl;

	// for (auto temp: Tr)
	// {
	// 	for (auto temp2:temp.second)
	// 		cout<<temp.first<<" -- "<<temp2.first<<" --> "<<temp2.second<<endl;
	// }
	// cout<<endl;

	myfile.close();

	string cur=In;
	for (int i=0;i<input.size();i++)
	{
		if (!Al[input[i]])
			throw input[i];
		cur=Tr[cur][input[i]];
	}
	if (Fi[cur])
		return true;
	else 
		return false;

}