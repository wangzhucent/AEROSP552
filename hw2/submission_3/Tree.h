#include <iostream>
#include <ostream>
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
	Node* root;
public:
	Tree();
	Tree(const Tree & mytree);
	~Tree();
	void insert(int x);
	void erase(int x);
	void print();
	bool member(int x);
	int size() const;
	int * const toarray() const;
	Tree treeUnion(Tree a, Tree b);
	Tree intersection(Tree a, Tree b);
	friend std::ostream & operator <<(std::ostream & os, const Tree& mytree);
};

std::ostream & operator <<(std::ostream & os, const Tree& mytree);
#endif