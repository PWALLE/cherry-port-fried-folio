//Paul Elliott, Revised 6 December 2009
//This file contains a class definition for the symbol table classes,
//which is used by the parser to manage the scanner.
//ADDED: get_temp_ptr
//Finished: declarations for proj6/7

//The definitions of these routines are found in table.cpp

#ifndef _table
#define _table

#include "GENERAL.H"
#include <iostream>
using namespace std;

class table_node
{
public:
	char node_lexeme[80];
	int kind;
	int type;
	int node_offset;
	int node_nesting_level;
	int numparams;
	table_node* parameters;
	table_node* next_node;
};

class table_level
{
public:
	char name[80];
	int nesting_level;
	int next_offset;
	table_node* list;
	table_level* next_level;
};

class table
{ public:
	table();
	void table_add_level(char level_name[]);
	void table_del_level();
	void table_add_entry(char sym_name[], int sym_kind, int sym_type);
	table_node* table_lookup(char compare[]);
	table_node* get_temp_ptr();
	void delete_temps();
	void delete_special_temps();
	void add_param(table_node* to_add);
	void print_symbol_table();
	void insert_func(char check[],int num);
	bool get_param(char func_check[],int check_offset,int &kind,int &type);
	void reset_offset();
	void insert_num_params(int p);
	int get_top_nesting_level();

	bool check_duplicate_id(char check_lex[]);
	void print(int to_print);

  private:
	int current_nesting_level;
	table_level* top;
};

#endif