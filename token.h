// Token Header file
// The tokenizer is an object that returns a linked list of object tokens, which are these.

#ifndef TOKEN
#define TOKEN

// Provides some helpful functionality for understanding tokens
#include <iostream>
#include <string>
using namespace std;

// Here is a definition of the token itself:
class Token {

    public:
	enum Kind { NONE, INTEGER, NAME, OPERATOR, PUNCT };

    private:
	Kind	kind;		// to identify the token type later
	int		value;		// value for an integer token
	char	oper;		// character for a punctuation token: ( ) ,
	string	str;		// name of a variable/function, or an operator such as "<="

    public:
	Token(char c)		// punctuation token
	{
	    kind = PUNCT;
	    oper = c;
	    value = 0;		// initialize unused value
	}

	Token(int i)		// integer token
	{
	    kind = INTEGER;
	    value = i;
	    oper = 0;		// initialize unused value
	}

	Token(string s, Kind k = NAME)	// name (default) or operator token
	{
		kind = k;
		str = s;
		value = 0;
		oper = 0;
	}

	Token()			// default constructor: the null token
	{
	    kind = NONE;
	    value = 0;
	    oper = 0;
	}

	bool isNull() const
	{
	    return kind == NONE;
	}

	bool isInteger() const
	{
	    return kind == INTEGER;
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
		return kind == NAME;
	}

	bool isOperator() const
	{
		return kind == OPERATOR;
	}

	string tokenName() const	// name or operator text ("" for other kinds)
	{
		return str;
	}

	friend ostream& operator <<( ostream& stream, const Token &t);
};

#endif
