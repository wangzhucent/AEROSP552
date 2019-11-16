#include <iostream>
#include <cmath>
#include <unordered_map>

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

bool automaton(string file, string input)
{
	ifstream myfile;
	string str;
	myfile.open(file);

	// unordered_map<char, int> Al, St, Fi;
	int Al[256];
	int St[65536];
	int Fi[65536];

	for (int i=0;i<256;i++)
		Al[i]=0;

	for (int i=0;i<256*256;i++)
	{
		St[i]=0;
		Fi[i]=0;
	}
	int In;

	char Tr[255][255][255];
	// for (int i=0;i<256*256;i++)
	// {
	// 	for (int j=0;j<256;j++)
	// 		Tr[i][j]=-1;
	// }

	// getline(myfile,str);
	// while(getline(myfile,str) && str[0]!='-')
	// {
	// 	int temp=0;
	// 	for (int i=0;i<str.size();i++)
	// 	{
	// 		if (str[i]!=' ')
	// 		{
	// 			temp=temp*256+str[i];
	// 			Al[temp]++;
	// 		}
	// 		else
	// 		{
	// 			temp=0;
	// 			continue;
	// 		}
	// 	}
	// 			// cout<<endl;
	// 	str.clear();
	// }

	// while(getline(myfile,str) && str[0]!='-')
	// {
	// 	// cout<<"!!"<<str.size();
	// 	int temp=0;
	// 	for (int i=0;i<str.size();i++)
	// 	{
	// 		if (str[i]!=' ')
	// 		{
	// 			temp=temp*256+str[i];
	// 			// cout<<str[i]<<" ";
	// 			St[temp]++;
	// 		}
	// 		else
	// 		{
	// 			temp=0;
	// 			continue;
	// 		}
	// 	}
	// 	// cout<<endl;
	// 	str.clear();		
	// }

	// while(getline(myfile,str) && str[0]!='-')
	// {
	// 	if (str.size()==1)
	// 		In=str[0];
	// 	else
	// 		In=str[0]*256+str[1];
	// 	// cout<<In<<endl;
	// 	str.clear();
	// }

	// while(getline(myfile,str) && str[0]!='-')
	// {
	// 	// cout<<"!!"<<str.size()<<endl;
	// 	int temp=0;
	// 	for (int i=0;i<str.size();i++)
	// 	{
	// 		if (str[i]!=' ')
	// 		{
	// 			temp=temp*256+str[i];
	// 			// cout<<str[i]<<" ";
	// 			Fi[temp]++;
	// 		}
	// 		else
	// 		{
	// 			temp=0;
	// 			continue;
	// 		}
	// 	}
	// 	// cout<<endl;
	// 	str.clear();		
	// }
	// while(getline(myfile,str) && str[0]!='-')
	// {
	// 	if (str[1]==' ')
	// 	{
	// 		if (str.size()<=12 || str[12]==' ')
	// 			Tr[str[0]][str[5]]=str[11];	
	// 		else
	// 			Tr[str[0]][str[5]]=str[11]*256+str[12];
	// 	}
	// 	else
	// 	{
	// 		if (str.size()<=13 || str[13]==' ')
	// 			Tr[str[0]*256+str[1]][str[6]]=str[12];
	// 		else
	// 			Tr[str[0]*256+str[1]][str[6]]=str[12]*256+str[13];

	// 	}
	// 	str.clear();	
	// }

	// for (int i=0;i<256*256;i++)
	// {
	// 	for (int j=0; j<256;j++)
	// 	{
	// 		if (St[i] && Al[j] && Tr[i][j]==-1)
	// 			throw i*256+j;
	// 	}
	// }


	// for (int i=0;i<256;i++)
	// {
	// 	if (Al[i])
	// 		cout<< (char)Al[i]<<" ";
	// }
	// cout<<endl;
	myfile.close();

	// int cur=In;
	// for (int i=0;i<input.size();i++)
	// {
	// 	cur=Tr[cur][input[i]];
	// }
	// if (Fi[cur])
	// 	return true;
	// else 
	// 	return false;

	return false;
}