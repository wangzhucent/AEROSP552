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
	// mytree.insert(6);
	mytree.erase(5);
	// mytree.erase(6);
	// cout<<mytree.root<<endl;
	// mytree.erase(6);
	// mytree.print();
	// mytree.print();
	// mytree.erase(5);
	// mytree.insert(8);
	// mytree.print();
	// mytree.insert(6);
	// mytree.print();
	// mytree.insert(9);
	// mytree.print();
	// mytree.insert(4);
	// mytree.print();

	cout<<"eraser test: "<<endl;
	// mytree.erase(5);
	// mytree.print();
	// mytree.erase(8); 
	// mytree.print();
	// mytree.erase(5);
	// mytree.print();
	// cout<<mytree.root->val<<endl;

	cout<<"member test"<<endl;
	// cout<<"memeber 4: "<<mytree.member(4)<<endl;
	return 1;

}