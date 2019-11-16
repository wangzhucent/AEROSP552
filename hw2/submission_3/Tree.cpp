#include <iostream>
#include <ostream>
#include <cstdlib>
#include <vector>
#include "Tree.h"
using std::cout;
using std::endl;
using std::ostream;
// using std::



Tree::Tree()
{
	root=NULL;
};


void insert_helper(int x, Node* nd);
void copyconstructor_helper(Node* p,Node* &root)
{
	if(p)
	{
		if (!root)
		{
			Node* ptr=new Node;
			ptr->val=p->val;
			root=ptr;
		}
		else
			insert_helper(p->val,root);
		copyconstructor_helper(p->left,root);
		copyconstructor_helper(p->right,root);
	}
	
}
Tree::Tree(const Tree & mytree)
{
	root=NULL;
	copyconstructor_helper(mytree.root,root);
};


Tree::~Tree()
{
	while(root)
	{
		erase(root->val);
	}
};


void print_helper(vector<int> &temp_vec,Node*root, int i)
{
	if(!root || i>=8)
		return;
	temp_vec[i]=root->val;
	print_helper(temp_vec, root->left,2*i);
	print_helper(temp_vec, root->right,2*i+1);
};

void Tree::print() 
{
	vector<int> temp_vec(8,-1);
	Node* p=root;
	int i=1;
	print_helper(temp_vec,root,i);
	for (i=1;i<8;i++)
	{
		cout<<temp_vec[i]<<"\t";
	}
	cout<<endl;
	for (i=1;i<8;i++)
	{
		cout<<i<<"\t";
	}
	cout<<endl;
	cout<<endl;
};



void insert_helper(int x, Node* nd)
{
	if (x>=nd->val)
	{

		if (!nd->right)
		{
			Node* ptr=new Node;
			ptr->val=x;
			nd->right=ptr;
			return;
		}
		else
			insert_helper(x,nd->right);
	}
	else 
	{
		if (!nd->left)
		{

			Node* ptr=new Node;
			ptr->val=x;
			nd->left=ptr;
			return;
		}
		else
			insert_helper(x,nd->left);
	}
	return;
};

void Tree::insert(int x)
{
	if (!root)
	{
		Node* ptr=new Node;
		ptr->val=x;
		root=ptr;
		return;
	}
	else
		insert_helper(x,root);
	return;
};


Node* parent_successor(int x, Node* root)
{
	if(!root->right)
		return NULL;
	Node* pre_ptr=root;
	Node* ptr=root->right;
	while(ptr->left)
	{
		pre_ptr=ptr;
		ptr=ptr->left;
	}
	return pre_ptr;
}

Node* successor(int x, Node* root)
{
	if(!root->right)
		return NULL;
	Node* ptr=root->right;
	while(ptr->left)
		ptr=ptr->left;
	return ptr;
}


void erase_helper(int x, Node* nd, Node* parent,Node* &root)
{
	if(!nd)
		return;

	if(nd->val==x)
	{
		if (!nd->left)
		{
			if (!parent)
			{
				root=nd->right;
			}
			else
			{
				if (nd==parent->left)
					parent->left=nd->right;
				else
					parent->right=nd->right;
			}
			delete nd;			
		}
		else if (!nd->right)
		{
			if (!parent)
				root=nd->left;
			else
			{
				if (nd==parent->left)
					parent->left=nd->left;
				else
					parent->right=nd->left;
			}
			delete nd;
		}
		else
		{
			Node* temp=successor(x,nd);
			
			Node* parent=parent_successor(x,nd);
		
			int temp_int=temp->val;
			temp->val=x;
			nd->val=temp_int;
			erase_helper(x,temp,parent,root);
		}
		
	}
	else if(nd->val<x)
	{
		erase_helper(x,nd->right,nd,root);
	}
	else
	{
		erase_helper(x,nd->left,nd,root);
	}
	return;
};

void Tree::erase(int x)
{
	// cout<<"!!"<<endl;
	if(!root)
		return;
	// Node* ptr=root;
	// Node* parent=NULL;
	erase_helper(x,root,NULL,root);
	return;
};

bool member_helper(int x, Node*root)
{
	if(!root)
		return false;

	if(root->val==x)
		return true;
	else if(x>root->val)
		return member_helper(x,root->right);
	else
		return member_helper(x,root->left);
};

bool Tree::member(int x)
{
	return member_helper(x,root);
};

int size_helper(Node *root)
{
	if(!root)
		return 0;

	return 1+size_helper(root->left)+size_helper(root->right);
};

int Tree::size() const
{
	return size_helper(root);
};


void insert_sort(int * array, int n, int i, int x);
void toarray_helper(int *array, int n, int &i, Node* p)
{
	if(p )
	{
		insert_sort(array,n,i,p->val);
		i++;
		toarray_helper(array,n,i,p->left);
		toarray_helper(array,n,i,p->right);
	}
}

int * const Tree::toarray() const
{
	int n=size();
	int * const array=new int[n];
	int i=0;
	toarray_helper(array, n, i, root);
	return array;
}

void insert_sort(int * array, int n, int i, int x)
{
	array[i]=x;
	for(int k=i-1;k>=0;k--)
	{
		if (x<array[k])
		{
			array[i]=array[k];
			array[k]=x;
			i=k;
		}
		else
			break;
	}
	return;
}

void operator_helper(ostream &os, Node*  root)
{
	if (!root)
		return;
	operator_helper(os,root->left);
	os<<(root->val)<<" ";
	operator_helper(os,root->right);
	return;
}

ostream &operator<<(ostream &os, const Tree  & mytree)
{
	Node* p=mytree.root;
	operator_helper(os,p);
	os<<endl;
	return os;
}

Tree Tree::treeUnion(Tree a, Tree b)
{
	Tree mytree(a);
	int * array;
	array=b.toarray();
	for (int i=0;i<b.size();i++)
	{
		if(!mytree.member(array[i]))
			mytree.insert(array[i]);
	}
	delete [] array;
	return mytree;
}

Tree Tree::intersection(Tree a, Tree b)
{
	Tree mytree;
	int * array;
	array=b.toarray();
	for (int i=0;i<b.size();i++)
	{
		if(a.member(array[i]))
			mytree.insert(array[i]);
	}
	delete [] array;
	return mytree;
}