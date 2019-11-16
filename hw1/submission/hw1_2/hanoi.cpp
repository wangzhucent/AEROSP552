#include <iostream>
#include "hanoi.h"
using namespace std;

void hanoi(int n)
{
	hanoi_helper(n,0,2);
}

void hanoi_helper(int n,int from, int to)
{
	if (n==1)
	{
		cout<<"Make a move from "<<(char)('A'+from)<< " to "<<(char)('A'+to)<<endl;
	}
	else
	{
		int i=0;
		for (i=0;i<3;i++)
		{
			if (from!=i && to !=i)
				break;
		}
		int mid=i;
		hanoi_helper(n-1,from,mid);
		cout<<"Make a move from "<<(char)('A'+from)<< " to "<<(char)('A'+to)<<endl;
		hanoi_helper(n-1,mid,to);
	}
	return;
}