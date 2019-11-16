#include "rate_control.h"
// #include "da.h"

using namespace std;

int main()
{
	int freq=100;
	double desiredVolt=2.38;
	double desiredTime=10;
	double P, I, D;
	int window_size;
	bool isMean;

	string FileName;

	cout<<"Please enter the file name for recording: "<<endl;
	cin>>FileName;

	cout<<"Please enter the sample frequency (int):"<<endl;
	cin>>freq;

	cout<<"Please enter the desired voltage of gyroscope: "<<endl;
	cin>> desiredVolt;

	cout<<"Please enter the total time: "<<endl;
	cin>> desiredTime;

	cout<<"Please enter the window size (int): "<<endl;
	cin>>window_size;

	cout<<"please enter if use window filter (bool):"<<endl;
	cin>>isMean;

	System_Class system(freq, desiredVolt, desiredTime, 10, 0, 0, window_size, isMean);
	system.daq(FileName);
}