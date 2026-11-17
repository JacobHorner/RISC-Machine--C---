// compile
// Tokenizes the string, converts to expr tree, and compiles that

#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;
#include "tokenlist.h"
#include "compile.h"
#include "exprtree.h"

static void createFun(ListIterator &, FunctionDef &);

// Throws a syntax error unless the condition holds
static void require(bool condition, const string &message, const Token &found)
{
	if (!condition)
	{
		ostringstream msg;
		msg << message << ", found ";
		if (found.isNull())
			msg << "end of line";
		else
			msg << "'" << found << "'";
		throw runtime_error(msg.str());
	}
}

void compile(const char expr[], VarTree &vars, FunctionDef &funs,
				Instruction *prog[], int &pBegin, int &pEnd)
{
	TokenList list(expr);		// initialized infix list
	if (list.empty())
		return;					// blank line

	ListIterator iter = list.begin();
	int lineStart = pEnd;		// so a failed line can be undone

	try
	{
		if (iter.token().isVariable() && iter.token().tokenName() == "deffn")
		{
			createFun(iter, funs);
			return;
		}

		ExprNode *root;
		equalsNode(root, iter);
		if (!iter.ended())
		{
			delete root;
			require(false, "unexpected input after expression", iter.token());
		}

		int progCounter = 1;	// temporary registers are reused for each line
		try
		{
			root->compile(vars, funs, prog, pBegin, pEnd, progCounter);
			emit(prog, pEnd, new Print(progCounter - 1));
		}
		catch (...)
		{
			delete root;
			throw;
		}
		delete root;
	}
	catch (runtime_error &e)
	{
		cout << "Error: " << e.what() << " (line skipped)" << endl;

		while (pEnd > lineStart)		// discard any code from this line
			delete prog[--pEnd];
		for (FunctionDef::iterator it = funs.begin(); it != funs.end(); ++it)
			it->second.expanding = false;
	}
}

// Parses  deffn name(param, ...) = expression  and records the function
static void createFun(ListIterator &iter, FunctionDef &funs)
{
	FunDef function;
	function.paramCount = 0;
	function.expanding = false;

	iter.advance();		// get past deffn
	require(iter.token().isVariable(), "expected a function name after deffn", iter.token());
	function.name = iter.token().tokenName();
	iter.advance();

	require(iter.token().tokenChar() == '(', "expected '(' after function name", iter.token());
	iter.advance();

	if (iter.token().tokenChar() != ')')
	{
		while (true)
		{
			require(iter.token().isVariable(), "expected a parameter name", iter.token());
			require(function.paramCount < 10, "too many parameters (at most 10)", iter.token());
			function.parameter[function.paramCount++] = iter.token().tokenName();
			iter.advance();
			if (iter.token().tokenChar() != ',')
				break;
			iter.advance();	// get past the comma
		}
	}
	require(iter.token().tokenChar() == ')', "expected ')' after parameters", iter.token());
	iter.advance();
	require(iter.token().tokenName() == "=", "expected '=' before function body", iter.token());
	iter.advance();

	ExprNode *tree;
	equalsNode(tree, iter);
	if (!iter.ended())
	{
		delete tree;
		require(false, "unexpected input after function body", iter.token());
	}

	FunctionDef::iterator old = funs.find(function.name);
	if (old != funs.end())
		delete old->second.functionBody;	// redefinition replaces the old body

	function.functionBody = tree;
	funs[function.name] = function;

	cout << "Defined " << function.name << "(";
	for (int i = 0; i < function.paramCount; i++)
		cout << (i > 0 ? ", " : "") << function.parameter[i];
	cout << ") = " << *tree << endl;
}
