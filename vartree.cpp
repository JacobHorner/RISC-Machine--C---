// Variable Tree Implementation File
// This is a simple binary search tree associating variables
// with integer values.
#include <iostream>
#include <string>
using namespace std;

#include "vartree.h"

//  recursiveAssign
//  A recursive tree-traversal function to assign to a variable.
void VarTree::recursiveAssign(TreeNode *&root, string name, int value, int loc)
{
	TreeNode *tempNode = root;

	if (root == NULL)
	{
		root = new TreeNode(name, value, loc);
	}
	else if (tempNode->name != name)
	{
		if (name < tempNode->name)
			recursiveAssign(tempNode->left, name, value, loc);
		else if (name > tempNode->name)
			recursiveAssign(tempNode->right, name, value, loc);
	}
	else
	{
		tempNode->value = value;
		treeSize++;
	}
		
}

//  assign
//  Assigns a value to a variable.
void VarTree::assign( string name, int value , int loc )
{
    recursiveAssign( root, name, value, loc );
}

//  recursiveLookup
//  A recursive tree-traversal function to search for a variable.
//  If the variable does not exist, it is created with a value of 0.
void VarTree::recursiveLookup( TreeNode *&root, string name, int &value, int &pos )
{
	if (root == NULL)
	{
		assign(name, 0, pos);
	}

	if (root->name != name)
		if (name < root->name)
			recursiveLookup(root->left, name, value, pos);
		else
			recursiveLookup(root->right, name, value, pos);
	else
	{
		value = root->value;
		pos = root->stackLocation;
	}
}

//  lookup
//  Searches for a variable to get its value
//  If the variable does not yet exist, it is created.
int VarTree::lookup( string name, int pos )
{
    int value;
    recursiveLookup( root, name, value, pos);
    return value;
}

int VarTree::lookupPos(string name, int pos)
{
	int value;
	recursiveLookup(root, name, value, pos);
	return pos;
}

int VarTree::size( )
{
	return treeSize;
}