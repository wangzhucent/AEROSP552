#include <iostream>
#include <sstream>
#include <fstream>
// #include <cstdlib>
#include <vector>
#include <string>

using namespace std;

void paint_world ( vector<vector<int> >World);
void life ();
void life_2(vector < vector <int > > & world);
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
	vector < vector <int> > World;
	vector<int> Line;
	ifstream myfile;
	myfile.open("life.dat");
	// cout<<myfile.good()<<endl;
	while(getline(myfile,str))
	{
		// vector<vector<int> > World_2;
		height++;
		Line.clear();
		if (-1==width)
		{
			width=str.length();
		}
		else 
		{
			if (width!=str.length())
			{
				cout<<"Wrong input! Different width of lines."<<endl;
				return;
			}
		}

		for (int i=0;i<width;i++)
		{
			if ('O'==str[i])
				Line.push_back(1);
			else if (' '==str[i])
				Line.push_back(0);
			else
			{
				cout<<"Wrong input! Illegal character."<<endl;
				return;
			}
		}
		World.push_back(Line);
	}

	// cout<<width<<" "<<height<<endl;
	// life(World);
	// paint_world(World);
	string temp_str;
	char mychar;
	//add a Flag

	life_2(World);
	paint_world(World);
	cout<<"Press <Enter> to continue or type quit"<<endl;
	// istringstream my_iss;
	while (cin.get(mychar))
	{
		
		// my_iss.str()
		// cout<<mychar<<endl;
		if (mychar=='\n')
		{
			life_2(World);
			paint_world(World);
			cout<<"Press <Enter> to continue or type quit"<<endl;
		}
		else if (mychar=='q')
		{
			getline(cin,temp_str);
			if (temp_str=="uit")
				return;
		}
		// else if ()
		
			
	}		
	return;
}

void life_2(  vector<vector<int> > & World)
// void life(World)
{
	int height=World.size();
	int width=World[0].size();
	vector<vector<int> > World_2(height,vector<int>(width));
	int i=0;
	int j=0;
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

	World=World_2;
	return;
}

void paint_world(vector<vector<int> > World)
{
	int height=World.size();
	int width=(World[0].size());

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