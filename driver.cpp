#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
#include "compile.h"

int main( int argc, char *argv[] )
{
    ifstream infile;
    string fileLine;
    VarTree vars;		// initially empty tree
    FunctionDef funs;
    Instruction *program[MAX_CODE];	// space for MAX_CODE instructions
    int stack[MAX_STACK] = { 0 };	// stack space for MAX_STACK values, all starting at 0
    int temps[MAX_TEMPS] = { 0 };	// up to MAX_TEMPS temporary registers

    int progBegin = 0;		// where to begin execution
    int progEnd = 0;		// where program ends (first unused spot)
    int stackPointer;		// pointer to stack memory
    int programCounter;		// pointer to instruction

    if (argc < 2)
    {
		cout << "Call this program with a name of a file afterwards" << endl;
		return 1;
    }

	infile.open( argv[1] );
	if (!infile)
	{
		cout << "Could not open " << argv[1] << endl;
		return 1;
	}

	while (getline( infile, fileLine ))
	{
		cout << endl << fileLine << endl;
		compile( fileLine.c_str(), vars, funs, program, progBegin, progEnd );
	}
	for (int i = 0; i < progEnd; i++)
		cout << setw(2) << i << ": " << *program[i] << endl;

	if (vars.size() > MAX_STACK)
	{
		cout << "Error: " << vars.size() << " variables do not fit in a stack of " << MAX_STACK << endl;
		return 1;
	}

	programCounter = progBegin;
	stackPointer = MAX_STACK - vars.size();
	while (programCounter < progEnd)
	{
		programCounter++;		// prepare for the next
		program[programCounter-1]->execute( 	// but execute this one
		temps, stack, stackPointer, programCounter );
	}

	for (int i = 0; i < progEnd; i++)
		delete program[i];
	return 0;
}
