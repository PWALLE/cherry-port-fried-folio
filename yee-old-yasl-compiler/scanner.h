//Paul Elliott, Revised 6 December 2009
//This file contains a class definition for lexical
//analysis, or pulling tokens out of the file to be compiled.
//Added: Compiler directive {$e<+->} = expression debugging,
//		 print_current_line() function to call Filemanager's
//		 print_current_line()
//Modified: Deleted printing of Compiler Directives,
//			made adjustments so PAREN and STREAMOP changes work
//Added: token functions display_type, display_subtype,
//		 and display_lexeme
//Added: declarations for functions calling symbol table functions
//Added: project6 functions
//Finished: project7 functions

//The definitions of these routines are found in scanner.cpp

#ifndef _scanner
#define _scanner

#include <string.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <ctype.h> //for get_next_state functionality
#include <stdlib.h>    //to allow exit
#include "filemngr.h"
#include "general.h"
#include "table.h"

using namespace std;
using std::ifstream;

class token_class
{ public:
	token_class();
	int get_type();
	int get_subtype();
	char* get_lexeme();
	void update(int new_type, int new_subtype, char new_lexeme[]);
    void display();
	void display_type();
	void display_subtype();
	void display_lexeme(); //for expression debugging
	void reset();

	char the_lexeme[80];
	table_node* symbol_ptr;

  private:
    int token_type;
	int token_subtype;
	map<int,string> the_map;
};

class scanner_class
{ public:
	scanner_class();
	void get_next_token(token_class &token);
	void add_char(char to_add);
	void check_keywords(token_class &token);
	void check_compiler_dir();
	void clear_buffer();
	void close_file();
	bool is_expression_debugging();
	void print_current_line();
	void s_print_lines_compiled();

	//Symbol table functions
	void s_table_add_level(char s_level_name[]);
	void s_table_del_level();
	void s_table_add_entry(char s_name[], int s_kind, int s_type);
	table_node* s_table_lookup(char s_compare[]);
	table_node* s_get_temp_ptr();
	void s_delete_temps();
	void s_delete_special_temps();
	void s_insert_func(char check[],int num);
	bool s_get_param(char func_check[],int check_offset,int &kind, int &type);
	void s_reset_offset();
	void s_insert_num_params(int p);
	int s_get_top_nesting_level();
  
  private:
	table the_table;
	file_manager_class the_file_mgr;
	char lexeme_buffer[80];
	int lexeme_length;
	bool expression_debugging;
};

//NON-MEMBER FUNCTIONS
int get_next_state(int row, char ch);

#endif
