// Token List Header file
// This is a linked list for use with the tokens for an
// arithmetic expression.  Although it is used for that
// particular application, this implementation more
// resembles a generic linked list, which may be used
// either as a stack or a queue.

#include <iostream>
using namespace std;
#include "token.h"

// Since this is a linked list, we actually need a linked list node.
struct ListElement {
    Token		token;		// the token data itself
    struct ListElement *next;		// next element of list
};

class TokenList  {
	friend class ListIterator;
    friend ostream& operator<<( ostream &, TokenList &);
    private:
	ListElement *head, 	// front of the list
	            *tail;	// tail of the list
	ListElement *current;	// facilitates traversal of the list
    public:
	TokenList()		// create an empty list
	{
	    head = NULL;
	    tail = NULL;
	    current = NULL;
	}

	TokenList( const char str[] );	// or create initial list
					// to appear in 'tokenlist.c'
				
	~TokenList()			// destructor 
	{
	    ListElement *remove;
	    while ( (remove = head) != NULL)
	    {
		head = head->next;	// find the successor
		delete remove;		// and deallocate this
	    }
	}
	
	void push_front(Token t);
	void push_back(Token t);

	void start()
	{
		current = head;
	}

	void advance()
	{
		current = current->next;
	}

	bool atEnd() const
	{
		return current == NULL;
	}

	Token nextToken() const
	{
		if (current == NULL)
		{
			Token zero;
			return zero;
		}
		return current->token;
	}

	Token first()
	{
		return Token(head->token);
	}

	ListIterator begin();
	ListIterator end();
};

class ListIterator
{
	friend class TokenList;	// let that class create iterators
private:
	TokenList *list;	// which list
	ListElement **curr;	// pointer to current node
	ListIterator(TokenList *l, ListElement **le)
	{
		list = l;
		curr = le;
	}
public:
	Token& token() const
	{
		return (*curr)->token;
	}
	bool ended() const
	{
		return (*curr == NULL);	// list ends with NULL
	}
	void advance()
	{
		if (*curr != NULL)
			curr = &(*curr)->next;
	}
	int operator !=(const ListIterator &other) const
	{
		return list != other.list || curr != other.curr;
	}
};