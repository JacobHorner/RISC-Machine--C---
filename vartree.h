// Variable Tree Header File

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
	int	value;		// variable value
	int stackLocation; //location of the var in the stack
	TreeNode *left,		// sub-tree for less than
		 *right;	// sub-tree for greater than

	TreeNode( string newName , int val , int sLoc )
	{
	    name.assign( newName );	// get the name
	    value = val;		// and the value
		stackLocation = sLoc;
	    left = right = NULL;	// no children
	}
	friend ostream& operator<<(ostream&, TreeNode &); //adds treenode cout capability
};

class VarTree
{
    private:
	TreeNode *root;
	int treeSize;
    public:
        VarTree()
	{
	    root = NULL;	
		treeSize = 0;
	}
	void assign( string, int, int );
	int lookup( string, int);
	int lookupPos(string, int);
	int size();

    private:		
	void recursiveAssign( TreeNode *&, string, int, int);
	void recursiveLookup( TreeNode *&, string, int &, int & );

	friend ostream& operator<<(ostream&, VarTree &);
};

#endif