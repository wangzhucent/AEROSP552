#include <iostream>
#include <cmath>
#include <assert.h>
#include "fly.h"
using namespace std;
void fly(double latA, double longA, double latB, double longB)
{
	const double R=3440;
	double a;
	double d;
	double theta;
	int Error_input=1;
	// assert(latA<=90. && latA>=-90. && longA<=180. && longA>=-180. && latB<=90. && latB>=-90. && longB<=180. && longB>=-180. );
	if (!(latA<=90. && latA>=-90. && longA<=180. && longA>=-180. && latB<=90. && latB>=-90. && longB<=180. && longB>=-180. ))
		throw Error_input;
	// if (!latA>=-90)
	// 	throw Error_input;
	double latA_rad=latA/180*M_PI;
	double longA_rad=longA/180*M_PI;
	double latB_rad=latB/180*M_PI;
	double longB_rad=longB/180*M_PI;
	a=pow(sin((latB_rad-latA_rad)/2),2)+cos(latA_rad)*cos(latB_rad)*pow(sin((longB_rad-longA_rad)/2),2);
	d=2*R*atan2(sqrt(a),sqrt(1-a));
	theta=atan2(sin(longB_rad-longA_rad)*cos(latB_rad),cos(latA_rad)*sin(latB_rad)-sin(latA_rad)*cos(latB_rad)*cos(longA_rad-longB_rad));
	theta=theta/M_PI*180;
	if (theta<0)
		theta+=360;
	cout<<"Fly heading "<<theta<<" degrees for "<<d<<" nautical miles."<<endl;
	return;
}