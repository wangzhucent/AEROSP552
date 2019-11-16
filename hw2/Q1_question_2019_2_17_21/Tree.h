#ifndef TREE_H
#define TREE_H 

struct Node
{
	int val;
	Node* left;
	Node* right;
};

class Tree
{
public:
	Node* root;
	Tree();
	// Tree(const Tree & mytree);
	~Tree();
	void insert(int x);
	void erase(int x);
	void print();
	bool member(int x);
	int size() const;
};

#endif