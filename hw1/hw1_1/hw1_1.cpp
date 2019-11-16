#include <iostream>
#include <cmath>
#include "fly.h"
#include <string>
using namespace std;

int main()
{
	double latA=0,longA=0,latB=90,longB=90;
	// cout<<atan2(10,0)<<endl;
	cout<<"Please input latitude of A:"<<endl;
	while (!(cin>>latA))
	{
		cin.clear();
		cin.ignore();
		cout<<"Wrong input!"<<endl;
	}
	cout<<"Please input longitude of A:"<<endl;
	while (!(cin>>longA))
	{
		cin.clear();
		cin.ignore();
		cout<<"Wrong input!"<<endl;
	}
	cout<<"Please input latitude of B:"<<endl;
	while (!(cin>>latB))
	{
		cin.clear();
		cin.ignore();
		cout<<"Wrong input!"<<endl;
	}
	cout<<"Please input longitude of B:"<<endl;
	while (!(cin>>longB))
	{
		cin.clear();
		cin.ignore();
		cout<<"Wrong input!"<<endl;
	}
	try 
	{
		fly(latA,longA,latB,longB);
	}
	catch (int v)
	{
		cout<<"Wrong input!"<<endl;
	}
	return 1;
}