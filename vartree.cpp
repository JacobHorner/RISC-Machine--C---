// Variable Tree Implementation File
// This is a simple binary search tree associating variables
// with their stack locations.
#include <iostream>
#include <string>
using namespace std;

#include "vartree.h"

VarTree::~VarTree()
{
	destroy(root);
}

void VarTree::destroy(TreeNode *node)
{
	if (node != NULL)
	{
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
}

VarTree *VarTree::global()
{
	VarTree *scope = this;
	while (scope->outer != NULL)
		scope = scope->outer;
	return scope;
}

const VarTree *VarTree::global() const
{
	const VarTree *scope = this;
	while (scope->outer != NULL)
		scope = scope->outer;
	return scope;
}

//  lookup
//  Searches this scope, then the enclosing scopes, for a variable.
bool VarTree::lookup( const string &name, int &stackLocation ) const
{
	TreeNode *node = root;
	while (node != NULL)
	{
		if (name < node->name)
			node = node->left;
		else if (name > node->name)
			node = node->right;
		else
		{
			stackLocation = node->stackLocation;
			return true;
		}
	}
	if (outer != NULL)
		return outer->lookup(name, stackLocation);
	return false;
}

//  declare
//  Adds a variable to this scope, giving it the next free stack slot.
//  Slots are counted in the global scope so no two variables share one.
int VarTree::declare( const string &name )
{
	int slot = global()->slotCount++;

	TreeNode **link = &root;
	while (*link != NULL)
	{
		if (name < (*link)->name)
			link = &(*link)->left;
		else if (name > (*link)->name)
			link = &(*link)->right;
		else
		{
			(*link)->stackLocation = slot;	// redeclared in this scope
			return slot;
		}
	}
	*link = new TreeNode(name, slot);
	return slot;
}

int VarTree::size() const
{
	return global()->slotCount;
}
