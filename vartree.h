// Variable Tree Header File
// A binary search tree associating variable names with
// their locations in the machine's stack memory.
// A tree may have an outer tree, so that a function's parameters
// can live in their own scope while still seeing global variables.

#include <iostream>
#include <string>
using namespace std;
#ifndef VARIABLE
#define VARIABLE


class TreeNode
{
    friend class VarTree;
    private:
	string	name;		// variable name
	int stackLocation;	// location of the var in the stack
	TreeNode *left,		// sub-tree for less than
		 *right;	// sub-tree for greater than

	TreeNode( string newName, int sLoc )
	{
	    name = newName;		// get the name
		stackLocation = sLoc;	// and where it lives
	    left = right = NULL;	// no children
	}
};

class VarTree
{
    private:
	TreeNode *root;
	VarTree *outer;		// enclosing scope (NULL for the global scope)
	int slotCount;		// stack slots handed out (only used in the global scope)

    public:
	VarTree( VarTree *enclosing = NULL )
	{
	    root = NULL;
		outer = enclosing;
		slotCount = 0;
	}
	~VarTree();

	// Finds a variable in this scope or any enclosing one.
	// Returns false if it has never been declared.
	bool lookup( const string &name, int &stackLocation ) const;

	// Declares a variable in this scope and returns its new stack slot.
	int declare( const string &name );

	// Total stack slots needed by every scope.
	int size() const;

    private:
	VarTree *global();
	const VarTree *global() const;
	static void destroy( TreeNode * );
	VarTree( const VarTree & );		// not copyable
	VarTree &operator=( const VarTree & );
};

#endif
