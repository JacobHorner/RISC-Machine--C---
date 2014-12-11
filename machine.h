//RISC machine
//This structure represents a simple machine that can compile
//arithmetic expressions.  In addition to the expected arithmetic
//operations, it includes relational operations and branching
//for the conditional operator, and simple support for function calls.
// The machine is expected to have a pre-allocated stack space
// for named variables and an arbitrarily long list of temporary
// registers, both represented by integer arrays here (to hold values).
// The machine also has a separate memory area holding all of the
// instructions defined below. Most of these instructions are also immutable.

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#ifndef INSTRUCTION
#define INSTRUCTION

class Instruction
{
   protected:
	int valueTemp;		// register computed or tested

	Instruction( int temp )
	{
	    valueTemp = temp;
	}
   public:																		//Had to output this way to avoid errors, per Roger's advice
	friend ostream& operator<<( ostream&, const Instruction & );
	virtual string toString() const = 0; // facilitates << operator
	virtual void execute(int[], int[], int &, int &) const {}
};


class Load : public Instruction													//Loads values into temp registers
{
protected:
	int varOffset;
public:
	Load(int reg, int varOff) : Instruction(reg), varOffset(varOff) 
	{
		varOffset = varOff;
	}
	string toString() const;
	void execute(int[], int[], int &, int &) const;
};

class Store : public Instruction												//Stores into variable stack
{
protected:
	int stackPos;  //position in stack
	int registry;  //temp register position
public:
	Store(int reg, int pos) : Instruction(reg), stackPos(pos)
	{
		stackPos = pos;
	}
	string toString() const;
	void execute(int[], int[], int &, int &) const;
};

class LoadVar : public Instruction												//Loads variables into temp registers
{
protected:
	int varOffset;
public:
	LoadVar(int reg, int varOff) : Instruction(reg), varOffset(varOff)
	{
		varOffset = varOff;
	}
	string toString() const;
	void execute(int[], int[], int &, int &) const;
};

class LoadTemp : public Instruction												//Loads a value into a temp register
{
protected:
	int registry2;
public:
	LoadTemp(int reg, int reg2) : Instruction(reg), registry2(reg2)
	{
		registry2 = reg2;
	}
	string toString() const;
	void execute(int[], int[], int &, int &) const;
};



class Condition : public Instruction					//support for the test for the conditional
{
protected:
	int lineNumber;
	Condition(int reg, int lineN) : Instruction(reg), lineNumber(lineN)
	{
		lineNumber = lineN;
	}
	virtual string toString() const = 0;
	virtual void execute(int[], int[], int &, int &) const {}
};

class FalseCase : public Condition						//support for the false case of the conditional
{
public:
	bool condition;
	string toString() const;
	void execute(int[], int[], int &, int &) const;
	FalseCase(int reg, int lineN) : Condition(reg, lineN){}
};

class TrueCase : public Condition					//support for the true case of the conditional
{
public:
	bool condition;
	string toString() const;
	void execute(int[], int[], int &, int &) const;
	TrueCase(int reg, int lineN) : Condition(reg, lineN){}
};



class Compute : public Instruction					//base for the computing iof the instructions
{
protected:
	int argA1, argB1;
	string oper1;
	Compute(int reg, int argA, int argB, string oper) : Instruction(reg)
	{
		argA1 = argA;
		argB1 = argB;
		oper1 = oper;
	}

	virtual string toString() const; // facilitates << operator
	virtual void execute(int[], int[], int &, int &) const {}
};

//support for all of the operators (including conditionals) for use in/by compute
class Add: public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Add(int result, int argA, int argB) :
		Compute(result, argA, argB, "+") {}
};

class Subtract : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Subtract(int result, int argA, int argB) :
		Compute(result, argA, argB, "-") { }
};

class Multiply : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Multiply(int result, int argA, int argB) :
		Compute(result, argA, argB, "*") { }
};

class Divide : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Divide(int result, int argA, int argB) :
		Compute(result, argA, argB, "/") { }
};

class Modulo : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Modulo(int result, int argA, int argB) :
		Compute(result, argA, argB, "%") { }
};

class Equals : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Equals(int result, int argA, int argB) :
		Compute(result, argA, argB, "==") { }
};

class NotEquals : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	NotEquals(int result, int argA, int argB) :
		Compute(result, argA, argB, "!=") { }
};

class Less : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Less(int result, int argA, int argB) :
		Compute(result, argA, argB, "<") { }
};

class LessEqual : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	LessEqual(int result, int argA, int argB) :
		Compute(result, argA, argB, "<=") { }
};

class Greater : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	Greater(int result, int argA, int argB) :
		Compute(result, argA, argB, ">") { }
};

class GreaterEqual : public Compute
{
public:
	void execute(int[], int[], int &, int &) const;
	GreaterEqual(int result, int argA, int argB) :
		Compute(result, argA, argB, ">=") { }
};

class Print : public Instruction //prints out the value stored into a register
{
public:
	string toString() const
	{
		ostringstream convert;
		convert << "print T" << valueTemp;
		return convert.str();
	}
	void execute(int[], int[], int &, int &) const;
	Print(int temp) : Instruction(temp) { }
};	 

#endif