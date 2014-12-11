//Expression Tree Header File
//Describes the elements of an expression tree, using
//derived classes to represent polymorphism.
//All objects in this structure are immutable 
//once constructed, they are never changed.
//They only be displayed or compiled.
#include <iostream>
using namespace std;
#include "vartree.h"
#include "funmap.h"
#include "machine.h"

class ExprNode
{
public:
	friend ostream& operator<<(ostream&, const ExprNode &);
	virtual string toString() const = 0;	// facilitates << operator
	virtual void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const = 0;  // compile this node
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
public:
	string toString() const;	// facilitates << operator
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	Operation(ExprNode *l, string o, ExprNode *r)
	{
		left = l;
		right = r;
		oper = o;
	}
};

class Conditional : public ExprNode
{
private:
	ExprNode *test, *trueCase, *falseCase;
public:
	string toString() const;	// facilitates << operator
	void compile(VarTree &v, FunctionDef &funs, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const;
	Conditional(ExprNode *b, ExprNode *t, ExprNode *f)
	{
		test = b;
		trueCase = t;
		falseCase = f;
	}
};

class Functional : public ExprNode
{
private:
	string name;
	int elements;
	ExprNode *parameters[10];
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
};