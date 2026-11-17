// Token List Implementation file
#include <string.h>
#include <string>
#include <ctype.h>
#include "tokenlist.h"

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
    ListElement *curr;
    for (curr = t.head; curr != NULL; curr = curr->next)
    {
	stream << " " << curr->token;
    }
    return stream;
}

static bool isDigit(char c)
{
	return isdigit((unsigned char)c) != 0;
}

static bool isLetter(char c)
{
	return isalpha((unsigned char)c) != 0;
}

// Two-character operators are checked before single-character ones,
// so that "a=-3" becomes  a  =  -  3  rather than  a  =-  3
static const char *twoCharOps[] = { "==", "!=", "<=", ">=" };
static const char oneCharOps[] = "+-*/%=<>?:";

TokenList::TokenList(const char str[])
{
	int number = 0, pos = 0;
	head = NULL;
	tail = NULL;
	current = NULL;
	while (str[pos] != '\0')
	{
		// Ignores spaces, tabs, and stray carriage returns
		if (isspace((unsigned char)str[pos]))
			pos++;

		// Creates integer tokens
		else if (isDigit(str[pos]))
		{
			number = 0;
			while (isDigit(str[pos]))
			{
				number = number * 10 + (int)(str[pos] - '0');
				pos++;
			}
			push_back(Token(number));	//create token and add to back of the list
		}

		// Combines letters (and any digits after the first letter) into a variable name
		else if (isLetter(str[pos]))
		{
			string var = "";
			while (isLetter(str[pos]) || isDigit(str[pos]))
			{
				var.push_back(str[pos]);
				pos++;
			}
			push_back(Token(var));
		}

		// Keeps parenthesis and commas as characters
		else if (str[pos] == '(' || str[pos] == ')' || str[pos] == ',')
		{
			push_back(Token(str[pos]));
			pos++;
		}

		// Operators: try two characters, then one.  Anything else is
		// kept as a one-character operator so the parser can report it.
		else
		{
			string op(1, str[pos]);
			if (str[pos + 1] != '\0')
			{
				string pair = op + str[pos + 1];
				for (int i = 0; i < 4; i++)
					if (pair == twoCharOps[i])
						op = pair;
			}
			push_back(Token(op, Token::OPERATOR));
			pos += op.length();
		}
	}
}

//Place tokens at the front of the list, making a new head.
void TokenList::push_front(Token t)
{
	ListElement *newNode = new ListElement;
	newNode->token = t;

	newNode->next = head; //Make the "space" after newNode the head of the list.
	head = newNode; //Make the head equal to the newNode.
	if (tail == NULL) //If the list was empty, this is also the tail.
		tail = newNode;
}

//Place tokens at the back of the list, making a new tail.
void TokenList::push_back(Token t)
{
	ListElement *newNode = new ListElement;
	newNode->token = t;
	newNode->next = NULL; //Nothing comes after the new tail.

	if (head == NULL) //If the list is empty...
		head = newNode;
	else
		tail->next = newNode; //Link the current tail to our new element.
	tail = newNode; //Make our newNode the new tail.
}

ListIterator TokenList::begin()
{
	return ListIterator(this, &head);
}

//  Creates an iterator to refer after the end of the list
ListIterator TokenList::end()
{
	return ListIterator(this, &tail->next);
}
