#include <iostream>
#include <cmath>
#include <unordered_map>
// #include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

bool automaton(string file, string input)
{
	ifstream myfile;
	string str;
	myfile.open(file);

	unordered_map<char, int> Al, St, Fi;
	char In;
	unordered_map<char, unordered_map<char, char> > Tr;
	getline(myfile,str);
	while(getline(myfile,str) && str[0]!='-')
	{
		for (int i=0;i<str.size();i++)
		{
			if (str[i]!=' ')
			{
				// cout<<str[i]<<" ";
				Al[str[i]]++;
			}
		}
				// cout<<endl;
		str.clear();
	}
	while(getline(myfile,str) && str[0]!='-')
	{
		// cout<<"!!"<<str.size();
		for (int i=0;i<str.size();i++)
			if (str[i]!=' ')
			{
				// cout<<str[i]<<" ";
				St[str[i]]++;
			}

		// cout<<endl;
		str.clear();		
	}

	while(getline(myfile,str) && str[0]!='-')
	{
		In=str[0];
		// cout<<In<<endl;
		str.clear();
	}

	while(getline(myfile,str) && str[0]!='-')
	{
		// cout<<"!!"<<str.size()<<endl;
		for (int i=0;i<str.size();i++)
			if (str[i]!=' ')
			{
				// cout<<str[i]<<" ";
				Fi[str[i]]++;
			}
		// cout<<endl;
		str.clear();		
	}
	while(getline(myfile,str) && str[0]!='-')
	{
		Tr[str[0]][str[5]]=str[11];	
		str.clear();	
	}


	// for (auto temp: Al)
	// {
	// 	cout<< temp.first<<" ";
	// }
	// cout<<endl;

	// for (auto temp: St)
	// {
	// 	cout<< temp.first<<" ";
	// }
	// cout<<endl;

	// for (auto temp: Fi)
	// {
	// 	cout<< temp.first<<" ";
	// }
	// cout<<endl;

	// for (auto temp: Tr)
	// {
	// 	for (auto temp2: temp.second)
	// 	{
	// 		cout<<temp.first<<"-";
	// 		cout<<temp2.first<<"->"<<temp2.second;
	// 		cout<<endl;
	// 	}
	// }
	myfile.close();

	char cur=In;
	for (int i=0;i<input.size();i++)
	{
		cur=Tr[cur][input[i]];
	}
	if (Fi[cur])
		return true;
	else 
		return false;
}