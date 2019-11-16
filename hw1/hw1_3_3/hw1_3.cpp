#include <iostream>
#include <sstream>
#include <fstream>
// #include <cstdlib>
// #include <vector>
#include <string>
#include "life.h"

using namespace std;
void paint_world ( int**World,int height, int width);
void life ();
int** life_2( int**World,int height, int width);

//Try a life_cur.dat for updating the input data

int main()
{
	life();
	return 1;
}
void life()
{
	int width=-1;
	int height=0;
	string str;
	// istringstream my_is;
	int**World;
	int* Line;
	ifstream myfile;
	myfile.open("life.dat");
	// cout<<myfile.good()<<endl;
	while(getline(myfile,str))
	{
		// vector<vector<int> > World_2;
		height++;
	}
	// delete World;
	World=new int*[height];
	myfile.close();
	myfile.open("life.dat");
	height=0;
	while(getline(myfile,str))
	{
		// vector<vector<int> > World_2;
		height++;
		
		if (-1==width)
		{
			width=str.length();
			Line=new int[width];
		}
		else 
		{
			Line=new int[width];
			if (width!=str.length())
			{
				cout<<"Wrong input! Different width of lines."<<endl;
				return;
			}
		}
		for (int i=0;i<width;i++)
		{
			if ('O'==str[i])
				Line[i]=1;
				
			else if (' '==str[i])
				Line[i]=0;
				
			else
			{
				cout<<"Wrong input! Illegal character."<<endl;
				
				return;
			}
		}
		World[height-1]=Line;
	}

	
	
	string temp_str;
	char mychar;
	World=life_2(World,height,width);
	paint_world(World,height,width);
	cout<<"Press <Enter> to continue or type quit"<<endl;
	while (cin.get(mychar))
	{
		
		if (mychar=='\n')
		{
			World=life_2(World,height,width);
			paint_world(World,height,width);
			cout<<"Press <Enter> to continue or type quit"<<endl;
		}
		else if (mychar=='q')
		{
			getline(cin,temp_str);
			if (temp_str=="uit")
			{
				for (int i=0;i<height;i++)
				{
					delete[] World[i];
				}
				delete[] World;

				return;
			}
		}
	}

	for (int i=0;i<height;i++)
	{
		delete[] World[i];
	}
	delete[] World;

	return;
}

int** life_2(  int ** World,int height,int width)
// void life(World)
{
	int** World_2=new int*[height];
	for (int i=0;i<height;i++)
		World_2[i]=new int[width];

	int i=0;
	int j=0;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
			World_2[i][j]=0;
	}

	int LivNgh=0;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			LivNgh=0;
			if (i-1>=0 && j-1>=0)
			{
				if (World[i-1][j-1]==1)
					LivNgh++;
			}
			if (i-1>=0)
			{
				if (World[i-1][j]==1)
					LivNgh++;
			}
			if (i-1>=0 && j+1<width)
			{
				if (World[i-1][j+1]==1)
					LivNgh++;
			}

			if ( j-1>=0)
			{
				if (World[i][j-1]==1)
					LivNgh++;
			}
			if ( j+1<width)
			{
				if (World[i][j+1]==1)
					LivNgh++;
			}

			if (i+1<height && j-1>=0)
			{
				if (World[i+1][j-1]==1)
					LivNgh++;
			}
			if (i+1<height)
			{
				if (World[i+1][j]==1)
					LivNgh++;
			}
			if (i+1<height && j+1<width)
			{
				if (World[i+1][j+1]==1)
					LivNgh++;
			}

			if (LivNgh<2)
				World_2[i][j]=0;
			else if (World[i][j]==1 && LivNgh>=2 && LivNgh<=3)
				World_2[i][j]=1;
			else if (LivNgh==3)
				World_2[i][j]=1;
			else if (LivNgh>=4)
				World_2[i][j]=0;
		}
	}
	for (i=0;i<height;i++)
	{
		delete[] World[i];
	}
	delete[] World;

	return World_2;
}

void paint_world(int** World,int height,int width)
{
	for (int i=0;i<height;i++)
	{
		for (int j=0;j<width;j++)
		{
			if (World[i][j]==1)
				cout<<'O';
			else
				cout<<'_';
		}
		cout<<'\n';
	}
	return;
}