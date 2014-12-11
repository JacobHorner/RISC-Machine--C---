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
	convert << "T" << valueTemp << " = " << varOffset;  
	return convert.str();
}

void Load::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	temps[valueTemp] = varOffset;
}

string Store::toString() const
{
	ostringstream convert;
	convert << "stack[" << stackPos << "] = T" << valueTemp;
	return convert.str();
}

void Store::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	stack[stackPointer] = temps[valueTemp];
}

string LoadVar::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = stack[" << varOffset << "]";
	return convert.str();
}

void LoadVar::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	temps[valueTemp] = stack[stackPointer];
}

string LoadTemp::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = T" << registry2;
	return convert.str();
}

void LoadTemp::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	temps[valueTemp] = temps[registry2];
}


string FalseCase::toString() const
{
	ostringstream convert;
	convert << "Go to " << lineNumber << " if T" << valueTemp << " false";
	return convert.str();
}

void FalseCase::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	if (temps[programCounter])
		programCounter = lineNumber;
}

string TrueCase::toString() const
{
	ostringstream convert;
	convert << "Go to " << lineNumber;
	return convert.str();
}

void TrueCase::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	programCounter = lineNumber - 1;
}


string Compute::toString() const
{
	ostringstream convert;
	convert << "T" << valueTemp << " = (T" << argA1 << oper1 << "T" << argB1 << ")";
	return convert.str();
}

void Add::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	temps[valueTemp] = temps[argA1] + temps[argB1];
}

void Subtract::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	temps[valueTemp] = temps[argA1] - temps[argB1];
}

void Multiply::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	temps[valueTemp] = temps[argA1] * temps[argB1];
}

void Divide::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	temps[valueTemp] = temps[argA1] / temps[argB1];
}

void Modulo::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	temps[valueTemp] = temps[argA1] % temps[argB1];
}

void Equals::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	if (temps[argA1] == temps[argB1])
		temps[programCounter+1] = false;
	else
		true;
}

void NotEquals::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{

}

void Less::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	
}

void LessEqual::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
}

void Greater::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	if (temps[argA1] > temps[argB1])
		temps[programCounter] = 0;
	else
		true;
}

void GreaterEqual::execute(int temps[], int stack[], int &stackCounter, int &programCounter) const
{
	
}

void Print::execute(int temps[], int stack[], int &stackPointer, int &programCounter) const
{
	cout << temps[valueTemp] << endl;
}