#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	ofstream file;
	cout<<" Please Enter the file name:"<<endl;
	string FileName;
	cin>> FileName;
	file.open(FileName.c_str());
	for (int i=0;i< 10;i++)
	{
		file<<i<<'\t'
		<<"123"<<'\t';
	}
	file<<endl;
	file.close();
}