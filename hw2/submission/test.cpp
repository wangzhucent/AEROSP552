#include <iostream>
#include "Tree.h"
using namespace std;

int main()
{
	cout<<1<<endl;
	Tree mytree;
	cout<<mytree.root<<endl;

	//Test insert
	mytree.insert(5);
	// mytree.print();
	mytree.insert(6);
	mytree.erase(5);
	mytree.erase(6);
	// cout<<mytree.root<<endl;
	mytree.erase(6);
	// mytree.print();
	// mytree.print();
	mytree.erase(5);
	mytree.insert(5);
	// mytree.print();

	mytree.insert(1);
	// mytree.print();
	mytree.insert(2);
	// mytree.print();
	// mytree.erase(1);
	// mytree.print();
	// mytree.erase(2);
	// cout<<(mytree.root==NULL)<<endl;
	// mytree.print();
	mytree.insert(9);
	// mytree.print();
	mytree.insert(8);
	mytree.print();

	// cout<<"erase test: "<<endl;
	// mytree.erase(5);
	// mytree.print();
	// mytree.erase(8); 
	// mytree.print();
	// mytree.erase(5);
	// mytree.print();
	// cout<<mytree.root->val<<endl;

	// cout<<"member test"<<endl;
	// cout<<"memeber 4: "<<mytree.member(4)<<endl;
	
	// cout<<"size test"<<endl;
	// mytree.print();
	// cout<<mytree.size()<<endl;

	cout<<"constructor test"<<endl;
	Tree mytree2(mytree);
	mytree2.print();
	

	// cout<<"toarray test"<<endl;
	// int* array;
	// array=mytree.toarray();
	// for (int i=0;i<mytree.size();i++)
	// {
	// 	cout<<array[i]<<" ";
	// }
	// cout<<endl;
	// delete [] array;

	// cout<<"<< test"<<endl;
	// cout<<mytree;

	cout<<"treeUnion test"<<endl;
	mytree2.erase(9);
	mytree2.erase(8);
	mytree2.insert(0);
	Tree newtree(mytree.treeUnion(mytree,mytree2));
	newtree.print();

	cout<<"intersection test"<<endl;
	Tree newtree2(mytree.intersection(mytree, mytree2));
	newtree2.print();
	return 1;
}