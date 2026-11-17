//  Token Implementation file
//  This stores some of the more complicated features of the token object,
//  that would not be compiled in-line.

#include "token.h"

ostream& operator<<( ostream &stream, const Token &t)
{
    if (t.isNull())
		return stream << "(null)";
	if (t.isVariable() || t.isOperator())
		return stream << t.str;
	if (t.isInteger())
		return stream << t.value;
	return stream << t.oper;
}
