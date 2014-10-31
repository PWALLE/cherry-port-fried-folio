//Paul Elliott, Revised 6 December 2009
//This file contains the definitions for operator precedence parsing,
//or rather checking to make sure the input expressions are
//valid YASL expressions
//Added: parse_program and subfunction statement,
//		 print_lines_compiled,
//		 first_S non-member function
//Modified: Got rid of first set function
//Added: statement grammar function declarations
//Added: program grammar function declarations
//Added: parse_error function declaration
//Added: undeclared_id function, and variables for symbol table
//		 and assembler output
//ADDED: declarations for functions running assembler output
//Modified:	Changed follow_id to have string parameter
//Finished: definitions and variables necessary for assembly language

//The definitions of these routines are found in parser.cpp

#ifndef _parser
#define _parser

#include "scanner.h"
#include "pstack.h"
#include "general.h"
#include <string>
#include <fstream>
#include <stdio.h>
using std::string;

class parser_class
{ public:
	parser_class();
	
	void parse_program();
	void program();
	void block();
	void var_decs();
	void type();
	void ident_list();
	void identifier_tail();
	void func_decs();
	void func_dec_tail();
	void param_list();
	void type_tail();
	void ident_tail();
	void prog_body();
	
	void statement();
	void follow_begin();
	void statement_tail();
	void follow_if(char if_label[], char else_label[]);
	void follow_id(char assigned_lex[]);
	void follow_expression(char assigned_lexeme[]);
	void follow_cin();
	void follow_cout();
	void cout_tail();
	void cout_output();
	void parse_expression();
	
	bool valid_expression(token_class expression[], int index, table_node* &save);
	void evaluate_expression(token_class to_eval[], table_node* &to_save);
	void print_production(token_class to_print[], int index);
	void print_lines_compiled();
	void print_instruction(char instr[], table_node* ptr1, char str1[], table_node* ptr2, char str2[]);
	void get_next_temp(char temp_name[]);
	void get_next_skip(char skip_name[]);
	void get_next_while(char top_while[], char end_while[]);
	void get_next_if(char false_if[], char false_else[]);
	void get_next_begin(char begin_name[]);
	void get_next_special_temp(char spec_name[]);

  private:
	table_node* pointer; //for symbol table lookup
    fstream outfile; //for assembler output
	scanner_class the_scanner;
	pstack the_stack;
	token_class the_token;

	//Variables necessary for symbol table functions
	int count_params;
	int params_used;
	int global_type;

	//Variables necessary for assembly output
	int count_globals;
	int expr_type;
	int expr_kind;
	bool isWhile;
	bool isIf;
	bool isFuncCall;
	int paramType;
	int paramKind;
	int nesting;
	
	void parse_error(token_class &found, string expected);
	void undeclared_id(token_class &found);
	void print(int to_print);
};

//NON-MEMBER FUNCTIONS
int precedence(token_class &row, token_class &column);


#endif