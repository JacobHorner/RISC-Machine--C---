// Simple Expression Compilation Header
// This describes the functionality for compiling some simple
// arithmetic expressions into a machine code representation.  
// Given a string consisting of operators and operands, 
// it will translate and store some representative operations.


#include "vartree.h"
#include "funmap.h"
#include "machine.h"

// Compile
// Compile the given expression into a machine code, with 
// the given variables defined New variables may be defined 
//when this function is called

void compile( const char expr[], VarTree &vars, FunctionDef &funs,
	Instruction *prog[], int &pBegin, int &pEnd );