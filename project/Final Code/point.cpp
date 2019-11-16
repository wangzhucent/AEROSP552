#include "project9.h"
// #include "da.h"

using namespace std;

int main()
{
	int freq=100;
	double desiredVal=2.30;
	double desiredTime=10;
	double P, I, D;
	int window_size=10;
	bool isMean=0;
	bool isRecord=0;
	bool isMonitor=0;
	double desiredAngle=0;

	string FileName;

	cout<<"Do you want to record the data in files?"<<endl;
	cin>>isRecord;

	if (isRecord)
	{
		cout<<"Please enter the file name for recording: "<<endl;
		cin>>FileName;
	}
	

	cout<<"Please enter the sample frequency (int):"<<endl;
	cin>>freq;

	cout<<"Please enter the desired angle of the satellite (0~359.999): "<<endl;
	cin>> desiredAngle;
	if (desiredAngle>180)
		desiredAngle-=360;
	

	cout<<"Please enter the total time: "<<endl;
	cin>> desiredTime;

	cout<<"Please enter the window size (int): "<<endl;
	cin>>window_size;

	cout<<"Please enter if use window filter (bool):"<<endl;
	cin>>isMean;

	cout<<"Please enter the value sof P, I, D: "<<endl;
	cin>>P>>I>>D;

	cout<<"Do you want to show the parameter in real time?"<<endl;
	cin>>isMonitor;

	System_Class system(freq, desiredVal, desiredTime, P, I, D, window_size, isMean);
	system.point(desiredAngle, FileName,isRecord,isMonitor);
}