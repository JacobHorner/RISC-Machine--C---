//Expression Tree Header File
//Describes the elements of an expression tree, using
//derived classes to represent polymorphism.
//All objects in this structure are immutable
//once constructed, they are never changed.
//They only be displayed or compiled.
//Parse and compile errors are reported by throwing runtime_error.

#ifndef EXPRTREE
#define EXPRTREE

#include <iostream>
#include <stdexcept>
using namespace std;
#include "vartree.h"
#include "funmap.h"
#include "machine.h"

class ListIterator;

class ExprNode
{
public:
	virtual ~ExprNode() {}
	friend ostream& operator<<(ostream&, const ExprNode &);
	virtual string toString() const = 0;	// facilitates << operator
	// compile this node; its result ends up in register progCounter-1
	virtual void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const = 0;
};

class Value : public ExprNode
{
private:
	int value;
public:
	string toString() const;	// facilitates << operator
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	Value(int v)
	{
		value = v;
	}
};

class Variable : public ExprNode
{
private:
	string name;
public:
	string toString() const;	// facilitates << operator
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	const string &getName() const
	{
		return name;
	}
	Variable(string var)
	{
		name = var;
	}
};

class Operation : public ExprNode
{
private:
	string oper;
	ExprNode *left, *right;	 // operands
	Operation(const Operation &);	// not copyable
	Operation &operator=(const Operation &);
public:
	string toString() const;	// facilitates << operator
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	Operation(ExprNode *l, string o, ExprNode *r)
	{
		left = l;
		right = r;
		oper = o;
	}
	~Operation()
	{
		delete left;
		delete right;
	}
};

class Conditional : public ExprNode
{
private:
	ExprNode *test, *trueCase, *falseCase;
	Conditional(const Conditional &);	// not copyable
	Conditional &operator=(const Conditional &);
public:
	string toString() const;	// facilitates << operator
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	Conditional(ExprNode *b, ExprNode *t, ExprNode *f)
	{
		test = b;
		trueCase = t;
		falseCase = f;
	}
	~Conditional()
	{
		delete test;
		delete trueCase;
		delete falseCase;
	}
};

class Functional : public ExprNode
{
private:
	string name;
	int elements;
	ExprNode *parameters[10];
	Functional(const Functional &);	// not copyable
	Functional &operator=(const Functional &);
public:
	string toString() const;
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	Functional(string n, int e, ExprNode *p[])
	{
		name = n;
		elements = 0;

		for (int i = 0; i < e; i++)
		{
			parameters[elements] = p[i];
			elements++;
		}
	}
	~Functional()
	{
		for (int i = 0; i < elements; i++)
			delete parameters[i];
	}
};

// Parses a whole expression (lowest precedence: assignment) from the token list
void equalsNode(ExprNode *&, ListIterator &);

// Adds an instruction to the program, checking that there is room for it
void emit(Instruction *prog[], int &pEnd, Instruction *instr);

#endif
