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

bool isInteger(char c)
{
	return isdigit(c);
}

TokenList::TokenList(const char str[])
{
	int number = 0, pos = 0;
	char oper = ' ';
	string var = "";
	int varCheck = 0;
	bool t = true;
	head = NULL;
	tail = NULL;
	while (str[pos] != '\0')
	{
		var = "";

		// Ignores spaces
		if (str[pos] == ' ')
			pos++;

		// Creates integer tokens
		else if (isInteger(str[pos]) == true)
		{
			number = (int)(str[pos] - '0');
			pos++;

			while (isInteger(str[pos]) == true)
			{
				number = number * 10 + (int)(str[pos] - '0');
				pos++;
			}

			Token num(number);
			push_back(num);		//create token and add to back of the list
		}

		// Converts string elements into a combined string and then stores as a variable name
		else if (isalpha(str[pos]))
		{
			while (isalpha(str[pos]))
			{
				var.push_back(str[pos]);
				pos++;
			}
			Token operand(var);
			push_back(operand);
		}

		// Keeps parenthesis as characters
		else if (str[pos] == '(' || str[pos] == ')' || str[pos] == ',')
		{
			Token operand(str[pos]);
			push_back(operand);
			pos++;
		}

		// Makes operators into their given strings
		else
		{
			while (!isInteger(str[pos]) && !isalpha(str[pos]) && str[pos] != ' ' && !(str[pos] == '(' || str[pos] == ')'))
			{
				var.push_back(str[pos]);
				pos++;
			}
			Token operand(var);
			push_back(operand);
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
}

//Place tokens at the back of the list, making a new tail.
void TokenList::push_back(Token t)
{
	ListElement *newNode = new ListElement;
	newNode->token = t;

	if (head == NULL && tail == NULL) //If the list is empty...
	{
		head = newNode;
		tail = newNode;
	}

	tail->next = newNode; //Set space after current tail to our new element, newNode.
	newNode->next = NULL; //Make the space directly after our newNode = NULL.
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
