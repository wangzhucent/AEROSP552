#include <iostream>
using namespace std;

struct intlist
{
	int head;
	struct intlist *next;
};



intlist* sort_helper(intlist* l)
{
	if (l==NULL || l->next==NULL)
		return l;

	intlist* l1=NULL;
	intlist* tail1=NULL;
	intlist* l2=NULL;
	intlist* tail2=NULL;
	intlist* mid=l;
	intlist* temp=l->next;
	bool hasFirst1=0, hasFirst2=0;
	while(temp)
	{
		if(temp->head<=mid->head)
		{
			if (hasFirst1++)
				tail1->next=temp;
			else
				l1=temp;
			tail1=temp;
		}
		else 
		{
			if (hasFirst2++)
				tail2->next=temp;
			else
				l2=temp;
			tail2=temp;
		}
		temp=temp->next;
	}
	if (tail1!=NULL)
		tail1->next=NULL;
	if (tail2!=NULL)
		tail2->next=NULL;

	l1=sort_helper(l1);
	l2=sort_helper(l2);
	if (l1==NULL)
	{
		mid->next=l2;
		return mid;
	}
	else
	{
		tail1=l1;
		while(tail1->next)
			tail1=tail1->next;
		tail1->next=mid;
		mid->next=l2;
		return l1;
	}
}


intlist* sort(intlist* l)
{
	if (l==NULL)
		return NULL;

	intlist* postemp=NULL;
	while(l)
	{
		intlist *temp=new intlist;
		temp->head=l->head;
		temp->next=postemp;
		postemp=temp;
		l=l->next;
	}	//Copy the original lists
	
	postemp=sort_helper(postemp);	// Sort the new list
	return postemp;	// Return the new list

}

intlist* push( intlist*l, int num)
{
	intlist* temp=new intlist;
	temp->head=num;
	temp->next=l;
	return temp;
};

intlist* pop(intlist*l)
{
	if (!l)
		return NULL;
	
	intlist* temp=l->next;
	delete l;
	return temp;
};

void print(intlist* l)
{
	intlist* temp=l;
	while(temp)
	{
		cout<<(temp->head);
		if (temp->next)
			cout<<"->";
		temp=temp->next;
	}
	cout<<endl;
};

void dele_list (intlist* l)
{
	intlist* temp=l;
	while(temp)
	{
		temp=temp->next;
		delete l;
		l=temp;
	}
};

int main()
{
	intlist* list=NULL;
	intlist* temp=NULL;


	// // Test #1: 9-3-1-5-2-4-8-20-7
	// list=push(list,7);
	// list=push(list,20);
	// list=push(list,8);
	// list=push(list,4);
	// list=push(list,2);
	// list=push(list,5);
	// list=push(list,1);
	// list=push(list,3);
	// list=push(list,9);
	// print(list);


	// // Test #2: 1-2-3-4-5-9
	// list=push(list,9);
	// list=push(list,5);
	// list=push(list,4);
	// list=push(list,3);
	// list=push(list,2);
	// list=push(list,1);
	// print(list);


	// // Test #3: 9-5-4-3-2-1
	// list=push(list,1);
	// list=push(list,2);
	// list=push(list,3);
	// list=push(list,4);
	// list=push(list,5);
	// list=push(list,9);
	// print(list);


	// // Test #4: 4-5-5-4-4-1
	// list=push(list,1);
	// list=push(list,4);
	// list=push(list,4);
	// list=push(list,5);
	// list=push(list,5);
	// list=push(list,4);
	// print(list);


	// // Test #5: 0
	// list=push(list,0);
	// print(list);


	// // Test #6: 0-0-0-0-0
	// list=push(list,0);
	// list=push(list,0);
	// list=push(list,0);
	// list=push(list,0);
	// list=push(list,0);
	// print(list);
	
	// // Test #7: []
	// print(list);

	// // Test #8: 1-2-3-4-5-9-8
	// list=push(list,8);
	// list=push(list,9);
	// list=push(list,5);
	// list=push(list,4);
	// list=push(list,3);
	// list=push(list,2);
	// list=push(list,1);
	// print(list);

	// // Test #9: 8-9-5-4-3-2-1
	// list=push(list,1);
	// list=push(list,2);
	// list=push(list,3);
	// list=push(list,4);
	// list=push(list,5);
	// list=push(list,9);
	// list=push(list,8);
	// print(list);


	temp=sort(list);
	print(temp);

	dele_list(list);
	dele_list(temp);
	return 1;
}