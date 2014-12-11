// Expression Tree 
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and compiled.
#include <iostream>
#include <sstream>
using namespace std;
#include "exprtree.h"
#include "tokenlist.h"

void equalsNode(ExprNode *&, ListIterator &); // convert equals
void condNode(ExprNode *&, ListIterator &);	// convert conditional
void boolNode(ExprNode *&, ListIterator &);	// convert boolean operators
void sumNode(ExprNode *&, ListIterator &);	// convert sum 
void prodNode(ExprNode *&, ListIterator &);	// convert product
void factorNode(ExprNode *&, ListIterator &);	// convert factor

ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

void Value::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const
{
	prog[pEnd++] = new Load(progCounter, value);
	progCounter++;
}

string Variable::toString() const
{
    return name;
}

void Variable::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const
{
	static int stackCounter = 0;
	if (v.lookup(name, stackCounter) == 0)
	{
		prog[pEnd++] = new Store(progCounter, stackCounter);
		v.assign(name, progCounter, stackCounter);
		stackCounter++;
	}
	else
	{
		prog[pEnd++] = new LoadVar(progCounter, v.lookupPos(name, stackCounter));
		progCounter++;
	}
}

string Operation::toString() const
{
	return "(" + left->toString() + " " + oper + " " + right->toString() + ")";
}

void Operation::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const //slightly modified from the previous function which created an expression tree.
{																														 //Now this function builds instruction lines using the order of operations already 
	int temp1 = 0;																										 //present in the compile function.
	int temp2 = 0;
	if (oper == "=")
	{
		right->compile(v, f, prog, pBegin, pEnd, progCounter);
		temp1 = progCounter - 1;
		left->compile(v, f, prog, pBegin, pEnd, temp1);
	}
	else
	{
		switch (oper[0])
		{
		case '+':
			left->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new Add(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '-': left->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new Subtract(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '*': left->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new Multiply(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '/': left->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new Divide(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '%': left->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new Modulo(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '=': left->compile(v, f, prog, pBegin, pEnd, progCounter); //support for conditional operators
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new Equals(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '!': left->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp1 = progCounter - 1;
			right->compile(v, f, prog, pBegin, pEnd, progCounter);
			temp2 = progCounter - 1;
			prog[pEnd++] = new NotEquals(progCounter, temp1, temp2);
			progCounter++;
			break;

		case '<':
			if (oper == "<")
			{
				left->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp1 = progCounter - 1;
				right->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp2 = progCounter - 1;
				prog[pEnd++] = new Less(progCounter, temp1, temp2);
				progCounter++;
			}
			else
			{
				left->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp1 = progCounter - 1;
				right->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp2 = progCounter - 1;
				prog[pEnd++] = new LessEqual(progCounter, temp1, temp2);
				progCounter++;
			}
			break;

		case '>':
			if (oper == ">")
			{
				left->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp1 = progCounter - 1;
				right->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp2 = progCounter - 1;
				prog[pEnd++] = new Greater(progCounter, temp1, temp2);
				progCounter++;
			}
			else
			{
				left->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp1 = progCounter - 1;
				right->compile(v, f, prog, pBegin, pEnd, progCounter);
				temp2 = progCounter - 1;
				prog[pEnd++] = new GreaterEqual(progCounter, temp1, temp2);
				progCounter++;
			}
			break;
		}
	}
}

string Conditional::toString() const
{
	ostringstream convert;
	convert << test;
	return convert.str();
}

// To compile, looks at test, if it returns 1 (true), return the compile of trueCase
// otherwise, return the compile of falseCase
void Conditional::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const //attempt at a compile for conditional with several variables to hold important
{																														   //registers to build conditional instructions with.
	int tempFalseStart = 0;
	int tempTrueStart = 0;
	int tempProgCounter = 0;
	int tempProgCounter2 = 0;
	int tempTrueGoTo = 0;
	bool cond = true;

	test->compile(v, f, prog, pBegin, pEnd, progCounter);
	tempProgCounter = progCounter - 1;
	tempFalseStart = pEnd++;
	trueCase->compile(v, f, prog, pBegin, pEnd, progCounter);
	tempTrueGoTo = pEnd++;
	tempTrueStart = pEnd++;
	falseCase->compile(v, f, prog, pBegin, pEnd, progCounter);
	tempProgCounter2 = progCounter - 1;
	prog[tempTrueGoTo] = new LoadTemp(tempProgCounter2, tempProgCounter + 1);
	prog[tempFalseStart] = new FalseCase(tempProgCounter, tempTrueStart + 1);
	prog[tempTrueStart] = new TrueCase(tempProgCounter, pEnd);
}

string Functional::toString() const //support for function not yet implemented
{
	//string retVal = name + "( " + argument[0]->toString();
	//for (int i = 1; i < argCount; i++)
	//	retVal = retVal + ", " + argument[i]->toString();
	//return retVal + " )";
	return "";
}

void Functional::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const
{
	VarTree newCall;
	//Functional fd = [name];
	//for (int i = 0; i < argCount; i++)
	//	newCall.assign(fd.parameter[i], argument[i]->evaluate(v, f));
	//return fd.functionBody->evaluate(newCall, f);
}

void equalsNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;

	condNode(tempLeft, infix);	//creates the expression tree for the left child

	if (!infix.ended() && infix.token().tokenName() == "=")
	{
		infix.advance();
		condNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, "=", tempRight);	// create the assignment node
	}

	root = tempLeft;
}

// condNode creates the conditional nodes
void condNode(ExprNode *&root, ListIterator &infix)
{
	//test, t, and f are temporary pointers
	ExprNode *test, *t, *f;
	string oper;		// possible operation

	boolNode(test, infix);	//creates the expression tree for the test

	if (!infix.ended())			// make sure there is more data
		oper = infix.token().tokenName();

	while (oper == "?")
	{
		infix.advance();
		boolNode(t, infix);	// create true case expression tree
		infix.advance();
		boolNode(f, infix);	// create false case expression tree
		test = new Conditional(test, t, f);	// create the conditional node

		if (infix.ended())			// no more operators
			oper = "";
		else
			oper = infix.token().tokenName();
	}

	root = test;
}

// boolNode creates the boolean operators nodes in the expression tree
void boolNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;
	string oper;			// possible operation

	sumNode(tempLeft, infix);	//creates the expression tree for the left child

	if (!infix.ended())			// make sure there is more data
		oper = infix.token().tokenName();

	while (oper == ">" || oper == "<" || oper == ">=" || oper == "<=" || oper == "==" || oper == "!=")
	{
		infix.advance();
		sumNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, oper, tempRight);	// create the boolean operator node

		if (infix.ended())			// no more operators
			oper = "";
		else
			oper = infix.token().tokenName();
	}

	root = tempLeft;
}

void sumNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;
	string oper;			// possible operation

	if (infix.token().tokenName() == "-")	// if negative
	{
		infix.advance();			//    go past the negative sign
		prodNode(tempRight, infix);	//       build a tree 
		tempLeft = new Operation(new Value(0), "-", tempRight);
	}
	else
		prodNode(tempLeft, infix);	//creates the expression tree for the left child

	if (!infix.ended())			// make sure there is more data
		oper = infix.token().tokenName();

	while (oper == "+" || oper == "-")
	{
		infix.advance();
		prodNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, oper, tempRight);	// create sum node

		if (infix.ended())			// no more operators
			oper = "";
		else
			oper = infix.token().tokenName();
	}

	root = tempLeft;
}

void prodNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;
	string oper;		// possible operation

	factorNode(tempLeft, infix);	//creates the expression tree for the left child

	if (!infix.ended())			// make sure there is more data
		oper = infix.token().tokenName();

	while (oper == "*" || oper == "/" || oper == "%")
	{
		infix.advance();
		factorNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, oper, tempRight);	// create product node

		if (infix.ended())			// no more operators
			oper = "";
		else
			oper = infix.token().tokenName();
	}

	root = tempLeft;
}

void factorNode(ExprNode *&root, ListIterator &infix)
{
	Token y = infix.token();
	if (infix.token().isInteger())
	{
		root = new Value(infix.token().integerValue());	// create integer node
		infix.advance();		// get past the number
	}
	else if (infix.token().isVariable())
	{
		char oper = ' ';
		string name = infix.token().tokenName();	// create name for possible function
		root = new Variable(infix.token().tokenName());	// create variable node
		infix.advance();	// get past variable

		if (infix.ended())			// no more operators
			oper = ' ';
		else
			oper = infix.token().tokenChar();

		if (oper == '(')	// check if variable is function name or not
		{
			int i = 0;	// counter for number of parameters
			ExprNode *parameters[10];	// array of exprnodes (pointers to parameters)
			infix.advance();

			while (infix.token().tokenChar() != ')')
			{
				equalsNode(root, infix);	// collects parameter as exprnode and ends at a comma
				parameters[i] = root;	// assign exprnode to parameter
				i++;	// add 1 to parameter counter
				if (infix.token().tokenChar() == ',')
					infix.advance();	// assume comma and get past it
			}
			root = new Functional(name, i, parameters);
		}
	}
	else
	{
		infix.advance();		// go past assumed (
		equalsNode(root, infix);
		infix.advance();		// go past assumed )
	}
}