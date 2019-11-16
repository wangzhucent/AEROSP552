#include <iostream>
#include "preflight.h"

using namespace std;

int main()
{
	int flt_state=0;
	flt_state=preflight();
	if (flt_state==1)
		cout<<"This plane is ready to fly"<<endl;
	else if (flt_state==0)
		cout<<"You cannot fly because [weight, moment, and/or center of gravity] is outside the operating envelope"<<endl;
	else if (flt_state==-1)
		cout<<"Wrong input"<<endl;
	// cout<<flt_state<<endl;
	return 1;
}