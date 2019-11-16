#include <iostream>
using namespace std;
#ifndef TREE_H
#define TREE_H 

struct Node
{
	int val;
	Node* left=NULL;
	Node* right=NULL;
};

class Tree
{
public:
	Node* root;
	Tree();
	Tree(const Tree & mytree);
	~Tree();
	void insert(int x);
	void erase(int x);
	void print();
	bool member(int x);
	int size() const;
	int * toarray();
	friend ostream &operator<<(ostream & os, const Tree& mytree);
	Tree treeUnion(Tree a, Tree b);
	Tree intersection(Tree a, Tree b);
	
};

ostream &operator<<(ostream & os,  Tree& mytree);
#endif