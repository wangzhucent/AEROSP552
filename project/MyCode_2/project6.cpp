#include "project6.h"
// #include "da.h"

using namespace std;

int main()
{
	int freq=100;
	double desiredVolt=2.38;
	double desiredTime=10;
	double P, I, D;

	string FileName;

	cout<<"Please enter the file name for recording: "<<endl;
	cin>>FileName;

	cout<<"Please enter the sample frequency (int):"<<endl;
	cin>>freq;

	cout<<"Please enter the desired voltage of gyroscope: "<<endl;
	cin>> desiredVolt;

	cout<<"Please enter the total time: "<<endl;
	cin>> desiredTime;

	System_Class system(freq, desiredVolt, desiredTime, 10, 0, 0);
	system.daq(FileName);
}