#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "preflight.h"
using namespace std;

int preflight()
{
	ifstream myfile;
	myfile.open("payload.dat");
	// cout<<"Is open "<<myfile.good()<<endl;
	istringstream my_iss;
	string str;
	double temp;
	vector<double> input(7);
	vector<double> arm(7);
	const double flt_wgt=1431;
	const double flt_arm=36.21;
	// double plt_wgt;
	// double frt_wgt;
	// double psng_wgt1;
	// double psng_wgt2;
	// double cag_wgt1;
	// double cag_wgt2;
	// double fuel_wgt;
	double weight=flt_wgt;
	double mmnt=flt_wgt*flt_arm;
	arm[0]=arm[1]=37;
	arm[2]=73;
	arm[3]=73;
	arm[4]=95;
	arm[5]=123;
	arm[6]=48;

	for (int i=0;i<7;i++)
	{

		if(getline(myfile,str))
		{
			// cout<<str<<endl;
			my_iss.clear();
			my_iss.str(str);
			my_iss>>temp;
			// cout<<temp<<endl;
			if (temp<0)
				return -1;
			if (i==0 && temp<=0)
				return -1;
			if (i==6)
			{
				input[i]=temp*6;
			}
			else
			{
				input[i]=temp;
			}
			// cout<<input[i]<<endl;
		}
		else 
			return -1;
			
	}

	for(int i=0;i<7;i++)
	{
		weight+=input[i];
		mmnt+=input[i]*arm[i];
	}

	double y=weight;
	double x=mmnt/1000;
	double x2=mmnt/weight;
	// cout<<y<<" "<<x<<" "<<x2<<endl;
	if (x>=52.5 && x<67.5 && y>(1950-1500)/(67.5-52.5)*(x-52.5)+1500)
		return 0;
	if (x>=67.5 && x<=88 && y>(2300-1950)/(88-67.5)*(x-67.5)+1950)
		return 0;
	if (x>=88 && x<=109 && y>2300)
		return 0;
	if (x>=70 && x<=109 && y<(2300-1500)/(109-70)*(x-70)+1500)
		return 0;
	if (x>=52.5 && x<=70 && y<1500)
		return 0;
	if (x<52.5 || x>109)
		return 0;

	if (x2>=35 && x2<=38.5 && y>(2300-1940)/(38.5-35)*(x-35)+1940)
		return 0;
	if (x2>=38.5 && x2<=47.3 && y>2300)
		return 0;
	if (x2>=35 && x2<=47.3 && y<1500)
		return 0;
	if (x2<35 || x2>47.3)
		return 0;

	return 1;






	return 1;
}