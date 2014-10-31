//Paul Elliott, Revised 5 October 2009
//This file contains a class definition for the pstack,
//which is used to run the operator precedence parser in
//parser.h, as well as the stack_cell objects that comprise
//the pstack.

//The definitions of these routines are found in pstack.cpp

#ifndef _pstack
#define _pstack

#include "scanner.h" //for token class
#include "table.h"

class stack_cell
{ public:
	int type;
	int subtype;
	char lex[80];
	stack_cell *next;
	table_node* ptr;
};

class pstack
{ public:
	pstack();
	void push(token_class token);
	token_class pop();
	int peek();
	int peek_top_terminal();
	token_class get_top_terminal();

  private:
	stack_cell *top;
};

#endif