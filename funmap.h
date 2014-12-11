// A data structure associating function names with their
// function bodies and parameter lists.   

#ifndef FUNMAP
#define FUNMAP

#include <map>

class ExprNode;						// declaring class names
class VarTree;						// for use below
struct FunDef
{
    string	name;			        // name of the function
    string	parameter[10];		    // parameter list
    VarTree    *locals;			    // parameters and local variables
    ExprNode   *functionBody;		// code for the function
};

typedef map <string, struct FunDef> FunctionDef;
#endif