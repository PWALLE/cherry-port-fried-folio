//Paul Elliott, Revised 5 September 2009
//This file contains method implementations for the
//pstack class

//The class header is found in pstack.h

#include "stdafx.h"
#include "pstack.h"

///////////////////////////////////////////////////
////           DEFINITION FOR PSTACK           ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=pstack
pstack::pstack()
//Precondition:  None.
//Postcondition: The pstack is initialized
{
	top = NULL;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=push
void pstack::push(token_class token)
//Precondition:  None.
//Postcondition: A new token is pushed onto the stack
{
	stack_cell *temp = new stack_cell;
	temp->type = token.get_type();
	temp->subtype = token.get_subtype();
	strcpy(temp->lex, token.get_lexeme());
	strcat(temp->lex,"\0");
	temp->ptr = token.symbol_ptr;
	temp->next = top;
	top = temp;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=pop
token_class pstack::pop()
//Precondition:  The stack is not empty
//Postcondition: The top of the stack is popped and returned
//				 as a token object
{
	if (top == NULL) {
		cout << "ERROR: Tried to pop empty stack" << endl;
		exit(1);
	}
	stack_cell *temp = top;
	top = top->next;
	token_class popped;
	popped.update(temp->type, temp->subtype, temp->lex);
	popped.symbol_ptr = temp->ptr;
	delete temp;
	return popped;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=peek
int pstack::peek()
//Precondition:  Stack is not empty
//Postcondition: Type of top of the stack is returned
{
	if (top == NULL) {
		cout << "ERROR: Tried to peek empty stack" << endl;
		cin.get();
		exit(1);
	}
	return top->type;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=peek_top_terminal
int pstack::peek_top_terminal()
//Precondition:  Stack is not empty
//Postcondition: Type of top terminal is returned
{
	if (top == NULL) {
		cout << "ERROR: Tried to peek empty stack" << endl;
		cin.get();
		exit(1);
	}
	stack_cell *iterator = top;
	while (iterator != NULL) {
		if (iterator->type != E_T) return iterator->type;
		iterator = iterator->next;
	}
	cout << "ERROR: No terminals on stack" << endl;
	exit(1);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_top_terminal
token_class pstack::get_top_terminal()
//Precondition:  Stack is not empty
//Postcondition: Topmost terminal on the stack is returned
//				 as a token_class object
{
	if (top == NULL) {
		cout << "ERROR: Tried to retrieve from empty stack" << endl;
		cin.get();
		exit(1);
	}
	stack_cell *iterator = top;
	while (iterator != NULL) {
		if (iterator->type != E_T) {
			token_class top_term;
			top_term.update(iterator->type, iterator->subtype, "\0");
			return top_term;
		}
		iterator = iterator->next;
	}
	cout << "ERROR: No terminals on stack" << endl;
	cin.get();
	exit(1);
}
///////////////////////////////////////////////////