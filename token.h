// Token Header file
// The tokenizer is an object that returns a linked list of object tokens, which are these.

#ifndef TOKEN
#define TOKEN

// Provides some helpful functionality for understanding tokens
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
using namespace std;
 
// Here is a definition of the token itself:
class Token {
   
    private:
	bool    isInt;		// to identify the token type later
	bool	isVar;		// Is variable
	bool	isOper;		// Is an operator in conditional expression
	int		value;		// value for an integer token
	char	oper;		// character for an operator token
	string	str;		// Handles the "names" of inputted variables.


    public:
	Token(char c)		//constructors 
	{			
	    oper = c;     
	    isInt = false;
		isVar = false;
	    value = 0;		// initialize unused value
	}

	Token(int i)
	{
	    value = i;
	    isInt = true;
		isVar = false;
	    oper = 0;		// initialize unused value
	}

	Token(string s) //String token constructor for (multi-character) variables
	{
		str = s;
		isInt = false;
		isVar = true;
		oper = 0;
	}

	Token()			// default constructor
	{
	    value = 0;
	    oper = 0;
	    isInt = false;
		isVar = false;
	}

	
	bool isNull() const
	{
	    return !isInt && oper==0 && !isVar && !isOper;
	}

	bool isInteger() const
	{
	    return isInt;
	}

	int integerValue() const
	{
	    return value;
	}
	
	char tokenChar() const
	{
	    return oper;
	}
	
	bool isVariable() const
	{
		return isVar;
	}

	string tokenName() const
	{
		return str;
	}

	friend ostream& operator <<( ostream& stream, Token &t);
};

#endif
