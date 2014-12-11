// compile
// Tokenizes the string, converts to expr tree, and compiles that

#include <iostream>
using namespace std;
#include "tokenlist.h"
#include <stack>
#include "compile.h"
#include "exprtree.h"

void equalsNode(ExprNode *&, ListIterator &); 
void createFun(ListIterator &, FunctionDef &);

void compile(const char expr[], VarTree &vars, FunctionDef &funs, 
				Instruction *prog[], int &pBegin, int &pEnd)
{
	static int progCounter = 1;
	static int result = 0;		// the return value
	Token prevValue(result);		// used only if necessary
	TokenList list(expr);		// initialized infix list
	ExprNode* root;

	//  If the first token is not a factor, we need to give it one
	Token y = list.first();
	if (!(list.first().isInteger() || list.first().isVariable() || list.first().tokenChar() == '('))
		list.push_front(prevValue);

	ListIterator iter = list.begin();

	string funcName = iter.token().tokenName();
	if (funcName == "deffn")
	{
		cout << "Original:  " << list << endl;
		createFun(iter, funs);
	}
	else if (funs.find(funcName) != funs.end())
	{
		int i = 0;
		iter.advance();	// assume name of function
		iter.advance(); // asusme left parenthesis
		while (iter.token().tokenChar() != ')')
		{
			equalsNode(root, iter);
			i++;
			if (iter.token().tokenChar() == ',')
				iter.advance();
		}
		
		
	}
	else
	{
		equalsNode(root, iter);	
		root->compile(vars, funs, prog, pBegin, pEnd, progCounter);
		prog[pEnd++] = new Print(progCounter - 1);
	}
}

void createFun(ListIterator &iter, FunctionDef &funs)
{
	int i = 0;
	ExprNode *tree;
	FunDef function;
	
	iter.advance();	
	function.name = iter.token().tokenName();
	iter.advance();	

	while (iter.token().tokenChar() != ')')
	{
		if (iter.token().isVariable() && i < 10)
		{
			function.parameter[i] = iter.token().tokenName();
			i++;
		}
		iter.advance();	
	}

	iter.advance();	
	iter.advance();	
	equalsNode(tree, iter);
	function.functionBody = tree;
	funs[function.name] = function;
}