// A data structure associating function names with their
// function bodies and parameter lists.

#ifndef FUNMAP
#define FUNMAP

#include <map>
#include <string>
using namespace std;

class ExprNode;						// declaring class name
									// for use below
struct FunDef
{
    string	name;			        // name of the function
    string	parameter[10];		    // parameter list
    int		paramCount;			    // how many parameters are used
    ExprNode   *functionBody;		// code for the function
    bool	expanding;			    // true while its body is being compiled
								    // (used to catch recursion)
};

typedef map <string, struct FunDef> FunctionDef;
#endif
