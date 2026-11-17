// Machine Implementation File
// This implementation represents a simple machine that can compile
// arithmetic expressions.  In addition to the expected arithmetic
// operations, it includes relational operations and branching
// for the conditional operator, and simple support for function calls.


#include <iostream>
#include <sstream>
using namespace std;
#include "machine.h"

ostream& operator<<( ostream &stream, const Instruction &i )
{
    return stream << i.toString();
}

string Load::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = " << constant;
	return convert.str();
}

void Load::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = constant;
}

string Store::toString() const
{
	ostringstream convert;
	convert << "stack[" << stackPos << "] = T" << valueTemp;
	return convert.str();
}

void Store::execute(int temps[], int stack[], int &stackPointer, int &) const
{
	stack[stackPointer + stackPos] = temps[valueTemp];
}

string LoadVar::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = stack[" << varOffset << "]";
	return convert.str();
}

void LoadVar::execute(int temps[], int stack[], int &stackPointer, int &) const
{
	temps[valueTemp] = stack[stackPointer + varOffset];
}

string LoadTemp::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = T" << registry2;
	return convert.str();
}

void LoadTemp::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = temps[registry2];
}


string FalseCase::toString() const
{
	ostringstream convert;
	convert << "Go to " << lineNumber << " if T" << valueTemp << " false";
	return convert.str();
}

void FalseCase::execute(int temps[], int[], int &, int &programCounter) const
{
	if (!temps[valueTemp])
		programCounter = lineNumber;
}

string TrueCase::toString() const
{
	ostringstream convert;
	convert << "Go to " << lineNumber;
	return convert.str();
}

void TrueCase::execute(int[], int[], int &, int &programCounter) const
{
	programCounter = lineNumber;
}


string Compute::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = (T" << argA1 << oper1 << "T" << argB1 << ")";
	return convert.str();
}

void Add::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = temps[argA1] + temps[argB1];
}

void Subtract::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = temps[argA1] - temps[argB1];
}

void Multiply::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = temps[argA1] * temps[argB1];
}

void Divide::execute(int temps[], int[], int &, int &) const
{
	if (temps[argB1] == 0)
	{
		cout << "Error: division by zero (result set to 0)" << endl;
		temps[valueTemp] = 0;
	}
	else
		temps[valueTemp] = temps[argA1] / temps[argB1];
}

void Modulo::execute(int temps[], int[], int &, int &) const
{
	if (temps[argB1] == 0)
	{
		cout << "Error: modulo by zero (result set to 0)" << endl;
		temps[valueTemp] = 0;
	}
	else
		temps[valueTemp] = temps[argA1] % temps[argB1];
}

void Equals::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = (temps[argA1] == temps[argB1]);
}

void NotEquals::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = (temps[argA1] != temps[argB1]);
}

void Less::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = (temps[argA1] < temps[argB1]);
}

void LessEqual::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = (temps[argA1] <= temps[argB1]);
}

void Greater::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = (temps[argA1] > temps[argB1]);
}

void GreaterEqual::execute(int temps[], int[], int &, int &) const
{
	temps[valueTemp] = (temps[argA1] >= temps[argB1]);
}

void Print::execute(int temps[], int[], int &, int &) const
{
	cout << temps[valueTemp] << endl;
}
