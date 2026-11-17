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

void condNode(ExprNode *&, ListIterator &);	// convert conditional
void boolNode(ExprNode *&, ListIterator &);	// convert boolean operators
void sumNode(ExprNode *&, ListIterator &);	// convert sum
void prodNode(ExprNode *&, ListIterator &);	// convert product
void factorNode(ExprNode *&, ListIterator &);	// convert factor

ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

// Describes a token for an error message
static string describe(const Token &t)
{
	if (t.isNull())
		return "end of line";
	ostringstream convert;
	convert << "'" << t << "'";
	return convert.str();
}

// Throws a syntax error unless the condition holds
static void expect(bool condition, const string &message, const Token &found)
{
	if (!condition)
		throw runtime_error(message + ", found " + describe(found));
}

void emit(Instruction *prog[], int &pEnd, Instruction *instr)
{
	if (pEnd >= MAX_CODE)
	{
		delete instr;
		throw runtime_error("program is too long for the machine's code memory");
	}
	prog[pEnd++] = instr;
}

// Hands out the next temporary register
static int newRegister(int &progCounter)
{
	if (progCounter >= MAX_TEMPS)
		throw runtime_error("expression needs too many temporary registers");
	return progCounter++;
}

string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

void Value::compile(VarTree &, FunctionDef &, Instruction *prog[], int &, int &pEnd, int &progCounter) const
{
	emit(prog, pEnd, new Load(newRegister(progCounter), value));
}

string Variable::toString() const
{
    return name;
}

// Reading a variable loads it from its stack slot.  A variable that
// has never been assigned is created on first use, holding 0.
void Variable::compile(VarTree &v, FunctionDef &, Instruction *prog[], int &, int &pEnd, int &progCounter) const
{
	int slot;
	if (!v.lookup(name, slot))
		slot = v.declare(name);
	emit(prog, pEnd, new LoadVar(newRegister(progCounter), slot));
}

string Operation::toString() const
{
	return "(" + left->toString() + " " + oper + " " + right->toString() + ")";
}

// Builds instruction lines using the order of operations already
// present in the expression tree.
void Operation::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const
{
	if (oper == "=")
	{
		// assignment: compute the right side, then store it into the variable.
		// The value stays in its register, so  a = 3  also prints 3.
		const Variable *target = dynamic_cast<const Variable *>(left);
		if (target == NULL)
			throw runtime_error("left side of '=' must be a variable");

		right->compile(v, f, prog, pBegin, pEnd, progCounter);
		int slot;
		if (!v.lookup(target->getName(), slot))
			slot = v.declare(target->getName());
		emit(prog, pEnd, new Store(progCounter - 1, slot));
		return;
	}

	left->compile(v, f, prog, pBegin, pEnd, progCounter);
	int temp1 = progCounter - 1;
	right->compile(v, f, prog, pBegin, pEnd, progCounter);
	int temp2 = progCounter - 1;
	int result = newRegister(progCounter);

	Instruction *instr;
	if      (oper == "+")  instr = new Add(result, temp1, temp2);
	else if (oper == "-")  instr = new Subtract(result, temp1, temp2);
	else if (oper == "*")  instr = new Multiply(result, temp1, temp2);
	else if (oper == "/")  instr = new Divide(result, temp1, temp2);
	else if (oper == "%")  instr = new Modulo(result, temp1, temp2);
	else if (oper == "==") instr = new Equals(result, temp1, temp2);	//support for conditional operators
	else if (oper == "!=") instr = new NotEquals(result, temp1, temp2);
	else if (oper == "<")  instr = new Less(result, temp1, temp2);
	else if (oper == "<=") instr = new LessEqual(result, temp1, temp2);
	else if (oper == ">")  instr = new Greater(result, temp1, temp2);
	else if (oper == ">=") instr = new GreaterEqual(result, temp1, temp2);
	else
		throw runtime_error("unknown operator '" + oper + "'");

	emit(prog, pEnd, instr);
}

string Conditional::toString() const
{
	return "(" + test->toString() + " ? " + trueCase->toString() + " : " + falseCase->toString() + ")";
}

// Compiles to:
//         <test>                      -> Ttest
//         Go to FALSE if Ttest false
//         <true case>                 -> Ttrue
//         Tresult = Ttrue
//         Go to END
// FALSE:  <false case>                -> Tfalse
//         Tresult = Tfalse
// END:
// The jump targets are not known until the cases are compiled,
// so those instructions are filled in afterwards.
void Conditional::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const
{
	test->compile(v, f, prog, pBegin, pEnd, progCounter);
	int testReg = progCounter - 1;
	int branchLine = pEnd;
	emit(prog, pEnd, NULL);			// placeholder for the branch

	trueCase->compile(v, f, prog, pBegin, pEnd, progCounter);
	int trueReg = progCounter - 1;
	int trueMoveLine = pEnd;
	emit(prog, pEnd, NULL);			// placeholder: Tresult = Ttrue
	int gotoEndLine = pEnd;
	emit(prog, pEnd, NULL);			// placeholder: jump past the false case

	int falseStart = pEnd;
	falseCase->compile(v, f, prog, pBegin, pEnd, progCounter);
	int falseReg = progCounter - 1;

	// the result register is allocated last, so it is progCounter-1 for our caller
	int result = newRegister(progCounter);
	emit(prog, pEnd, new LoadTemp(result, falseReg));

	prog[branchLine] = new FalseCase(testReg, falseStart);
	prog[trueMoveLine] = new LoadTemp(result, trueReg);
	prog[gotoEndLine] = new TrueCase(result, pEnd);
}

string Functional::toString() const
{
	string retVal = name + "(";
	for (int i = 0; i < elements; i++)
	{
		if (i > 0)
			retVal += ", ";
		retVal += parameters[i]->toString();
	}
	return retVal + ")";
}

// Function calls are expanded inline: each argument is computed and
// stored into a fresh stack slot for its parameter, and then the
// function body is compiled with those parameters in scope.
// Because of this, a function may not call itself (directly or indirectly).
void Functional::compile(VarTree &v, FunctionDef &f, Instruction *prog[], int &pBegin, int &pEnd, int &progCounter) const
{
	FunctionDef::iterator it = f.find(name);
	if (it == f.end())
		throw runtime_error("unknown function '" + name + "'");

	FunDef &fd = it->second;
	if (fd.paramCount != elements)
	{
		ostringstream msg;
		msg << "'" << name << "' takes " << fd.paramCount << " argument(s), but was given " << elements;
		throw runtime_error(msg.str());
	}
	if (fd.expanding)
		throw runtime_error("recursive call to '" + name + "' is not supported (functions are expanded inline)");

	int argRegs[10];
	for (int i = 0; i < elements; i++)
	{
		parameters[i]->compile(v, f, prog, pBegin, pEnd, progCounter);
		argRegs[i] = progCounter - 1;
	}

	VarTree locals(&v);		// parameters, with the caller's variables visible behind them
	for (int i = 0; i < elements; i++)
		emit(prog, pEnd, new Store(argRegs[i], locals.declare(fd.parameter[i])));

	fd.expanding = true;
	fd.functionBody->compile(locals, f, prog, pBegin, pEnd, progCounter);
	fd.expanding = false;
}

void equalsNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;

	condNode(tempLeft, infix);	//creates the expression tree for the left child

	if (infix.token().isOperator() && infix.token().tokenName() == "=")
	{
		if (dynamic_cast<Variable *>(tempLeft) == NULL)
		{
			delete tempLeft;
			throw runtime_error("left side of '=' must be a variable");
		}
		infix.advance();
		equalsNode(tempRight, infix);	// right side; also allows  a = b = 3
		tempLeft = new Operation(tempLeft, "=", tempRight);	// create the assignment node
	}

	root = tempLeft;
}

// condNode creates the conditional nodes:  test ? trueCase : falseCase
void condNode(ExprNode *&root, ListIterator &infix)
{
	//test, t, and f are temporary pointers
	ExprNode *test, *t, *f;

	boolNode(test, infix);	//creates the expression tree for the test

	if (infix.token().isOperator() && infix.token().tokenName() == "?")
	{
		infix.advance();
		condNode(t, infix);	// create true case expression tree
		expect(infix.token().tokenName() == ":", "expected ':' in conditional", infix.token());
		infix.advance();
		condNode(f, infix);	// create false case expression tree (a ? b : c ? d : e nests to the right)
		test = new Conditional(test, t, f);	// create the conditional node
	}

	root = test;
}

static bool isRelational(const Token &t)
{
	string oper = t.tokenName();
	return t.isOperator() &&
		(oper == ">" || oper == "<" || oper == ">=" || oper == "<=" || oper == "==" || oper == "!=");
}

// boolNode creates the boolean operators nodes in the expression tree
void boolNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;

	sumNode(tempLeft, infix);	//creates the expression tree for the left child

	while (isRelational(infix.token()))
	{
		string oper = infix.token().tokenName();
		infix.advance();
		sumNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, oper, tempRight);	// create the boolean operator node
	}

	root = tempLeft;
}

void sumNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;

	if (infix.token().isOperator() && infix.token().tokenName() == "-")	// if negative
	{
		infix.advance();			//    go past the negative sign
		prodNode(tempRight, infix);	//       build a tree
		tempLeft = new Operation(new Value(0), "-", tempRight);
	}
	else
		prodNode(tempLeft, infix);	//creates the expression tree for the left child

	while (infix.token().isOperator() &&
		(infix.token().tokenName() == "+" || infix.token().tokenName() == "-"))
	{
		string oper = infix.token().tokenName();
		infix.advance();
		prodNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, oper, tempRight);	// create sum node
	}

	root = tempLeft;
}

void prodNode(ExprNode *&root, ListIterator &infix)
{
	ExprNode *tempLeft, *tempRight;

	factorNode(tempLeft, infix);	//creates the expression tree for the left child

	while (infix.token().isOperator() &&
		(infix.token().tokenName() == "*" || infix.token().tokenName() == "/" || infix.token().tokenName() == "%"))
	{
		string oper = infix.token().tokenName();
		infix.advance();
		factorNode(tempRight, infix);	//creates the expression tree for the right child
		tempLeft = new Operation(tempLeft, oper, tempRight);	// create product node
	}

	root = tempLeft;
}

// A factor is a number, a variable, a function call, or a parenthesized expression
void factorNode(ExprNode *&root, ListIterator &infix)
{
	if (infix.token().isInteger())
	{
		root = new Value(infix.token().integerValue());	// create integer node
		infix.advance();		// get past the number
	}
	else if (infix.token().isVariable())
	{
		string name = infix.token().tokenName();	// variable or function name
		infix.advance();	// get past the name

		if (infix.token().tokenChar() != '(')	// check if variable is function name or not
		{
			root = new Variable(name);	// create variable node
			return;
		}

		int i = 0;	// counter for number of parameters
		ExprNode *parameters[10];	// array of exprnodes (pointers to parameters)
		infix.advance();	// get past (

		if (infix.token().tokenChar() != ')')
		{
			while (true)
			{
				expect(i < 10, "too many arguments to '" + name + "' (at most 10)", infix.token());
				equalsNode(parameters[i], infix);	// collects parameter as exprnode
				i++;	// add 1 to parameter counter
				if (infix.token().tokenChar() != ',')
					break;
				infix.advance();	// get past the comma
			}
		}
		expect(infix.token().tokenChar() == ')', "expected ')' after arguments to '" + name + "'", infix.token());
		infix.advance();	// get past )
		root = new Functional(name, i, parameters);
	}
	else if (infix.token().tokenChar() == '(')
	{
		infix.advance();		// go past (
		equalsNode(root, infix);
		expect(infix.token().tokenChar() == ')', "expected ')'", infix.token());
		infix.advance();		// go past )
	}
	else
		expect(false, "expected a number, variable, or '('", infix.token());
}
