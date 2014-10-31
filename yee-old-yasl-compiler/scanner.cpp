//Paul Elliott, Revised 6 December 2009
//This file contains method implementations for the
//scanner class and token class, as well as the
//non-member function get_next_state
//Added: Implementations for token functions 
//		 display_type, display_subtype, and display_lexeme
//Added: Implementations for functions calling symbol table functions
//Added: Implementations for proj6 functions
//Finished: Implementations for proj7

//The class header is found in scanner.h

#include "stdafx.h"  // Required for visual studio
#include <sstream>	 // Required to print the strings from the_map
using std::string;
#include "scanner.h"

///////////////////////////////////////////////////
////            DEFINITION FOR TOKEN           ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=token_class
token_class::token_class()
//Precondition:  None.
//Postcondition: The token is initialized and fields are updated,
//				 printing map is initialized
{
	token_type = NONE_T;
	token_subtype = NONE_T;
	the_lexeme[0] = '\0';
	symbol_ptr = NULL;

	//load map
	the_map.insert(pair<int,string>(STRING_T,"STRING_T"));
	the_map.insert(pair<int,string>(DIGIT_T,"DIGIT_T"));
	the_map.insert(pair<int,string>(IDENTIFIER_T,"IDENTIFIER_T"));
	the_map.insert(pair<int,string>(ASSIGN_T,"ASSIGN_T"));
	the_map.insert(pair<int,string>(ADDOP_T,"ADDOP_T"));
	the_map.insert(pair<int,string>(MULOP_T,"MULOP_T"));
	the_map.insert(pair<int,string>(RELOP_T,"RELOP_T"));
	the_map.insert(pair<int,string>(COMMA_T,"COMMA_T"));
	the_map.insert(pair<int,string>(SEMICOLON_T,"SEMICOLON_T"));
	the_map.insert(pair<int,string>(PERIOD_T,"PERIOD_T"));
	the_map.insert(pair<int,string>(OPENING_T,"OPENING_T"));
	the_map.insert(pair<int,string>(CLOSING_T,"CLOSING_T"));
	the_map.insert(pair<int,string>(INSERT_T,"INSERT_T"));
	the_map.insert(pair<int,string>(EXTRACT_T,"EXTRACT_T"));
	the_map.insert(pair<int,string>(REFERENCE_T,"REFERENCE_T"));
	the_map.insert(pair<int,string>(SWAP_T,"SWAP_T"));
	the_map.insert(pair<int,string>(IF_T,"IF_T"));
	the_map.insert(pair<int,string>(THEN_T,"THEN_T"));
	the_map.insert(pair<int,string>(ELSE_T,"ELSE_T"));
	the_map.insert(pair<int,string>(WHILE_T,"WHILE_T"));
	the_map.insert(pair<int,string>(DO_T,"DO_T"));
	the_map.insert(pair<int,string>(PROGRAM_T,"PROGRAM_T"));
	the_map.insert(pair<int,string>(FUNCTION_T,"FUNCTION_T"));
	the_map.insert(pair<int,string>(BEGIN_T,"BEGIN_T"));
	the_map.insert(pair<int,string>(END_T,"END_T"));
	the_map.insert(pair<int,string>(COUT_T,"COUT_T"));
	the_map.insert(pair<int,string>(CIN_T,"CIN_T"));
	the_map.insert(pair<int,string>(ENDL_T,"ENDL_T"));
	the_map.insert(pair<int,string>(INT_T,"INT_T"));
	the_map.insert(pair<int,string>(BOOLEAN_T,"BOOLEAN_T"));
	the_map.insert(pair<int,string>(TRUE_T,"TRUE_T"));
	the_map.insert(pair<int,string>(FALSE_T,"FALSE_T"));
	the_map.insert(pair<int,string>(NONE_T,"NONE_T"));
	the_map.insert(pair<int,string>(EOF_T,"EOF_T"));
	the_map.insert(pair<int,string>(E_T,"E_T"));
	the_map.insert(pair<int,string>(EQUAL_ST,"EQUAL_ST"));
	the_map.insert(pair<int,string>(NOTEQUAL_ST,"NOTEQUAL_ST"));
	the_map.insert(pair<int,string>(LESS_ST,"LESS_ST"));
	the_map.insert(pair<int,string>(LESSEQ_ST,"LESSEQ_ST"));
	the_map.insert(pair<int,string>(GREAT_ST,"GREAT_ST"));
	the_map.insert(pair<int,string>(GREATEQ_ST,"GREATEQ_ST"));
	the_map.insert(pair<int,string>(PLUS_ST,"PLUS_ST"));
	the_map.insert(pair<int,string>(MINUS_ST,"MINUS_ST"));
	the_map.insert(pair<int,string>(OR_ST,"OR_ST"));
	the_map.insert(pair<int,string>(AND_ST,"AND_ST"));
	the_map.insert(pair<int,string>(DIV_ST,"DIV_ST"));
	the_map.insert(pair<int,string>(MOD_ST,"MOD_ST"));
	the_map.insert(pair<int,string>(MULTIPLY_ST,"MULTIPLY_ST"));
	the_map.insert(pair<int,string>(NONE_ST,"NONE_ST"));
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_type
int token_class::get_type()
//Precondition:  None.
//Postcondition: Returns type of token (used mainly for loop in main())
{
	return token_type;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_subtype
int token_class::get_subtype()
//Precondition:  None.
//Postcondition: Returns subtype of token
{
	return token_subtype;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_lexeme
char* token_class::get_lexeme()
//Precondition:  None.
//Postcondition: Returns lexeme of token
{
	return the_lexeme;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=update
void token_class::update(int new_type, int new_subtype, char new_lexeme[])
//Precondition:  None.
//Postcondition: updates all of the token's fields
{
	token_type = new_type;
	token_subtype = new_subtype;
	strcpy(the_lexeme, new_lexeme);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=display
void token_class::display()
//Precondition:  The token is not of type NONE_T (a comment)
//Postcondition: The token is printed to screen in standard format
{
	if (token_type != NONE_T) {
		cout << setw(52) << the_lexeme << setw(14) << the_map[token_type] << setw(13) << the_map[token_subtype] << endl;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=display_type
void token_class::display_type()
//Precondition:  None.
//Postcondition: The token's type is printed to the screen
{
	cout << the_map[token_type];
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=display_subtype
void token_class::display_subtype()
//Precondition:  None.
//Postcondition: The token's subtype is printed to the screen
{
	cout << the_map[token_subtype];
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=display_lexeme
void token_class::display_lexeme()
//Precondition:  None.
//Postcondition: The token's lexeme is printed to the screen
{
	cout << the_lexeme;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=reset
void token_class::reset()
//Precondition:  None.
//Postcondition: Token's values are reset
{
	token_type = NONE_T;
	token_subtype = NONE_ST;
	the_lexeme[0] = '\0';
	symbol_ptr = NULL;
}
///////////////////////////////////////////////////

///////////////////////////////////////////////////
////          DEFINITION FOR SCANNER           ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=scanner_class
scanner_class::scanner_class()
//Precondition:  None.
//Postcondition: The constructor has initialized the private fields
//				 preset the lexeme_length to 0
{
	clear_buffer();
	expression_debugging = false;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_token
void scanner_class::get_next_token(token_class &token)
//Precondition:  The source file associated with the owning object has
//               been prepared for reading.
//Postcondition: Next token is pulled from the source file and returned
{
	token.reset();
	int state = 0;
	do {
		char check = the_file_mgr.get_next_char();
		state = get_next_state(state, check);
		switch (state) {
			case 0: break;
			case 1: if (lexeme_length < 50) {
						if (check != '\'') add_char(check);
					}
					else { //ERROR: too long string
						cout << "LEXICAL ERROR: String length exceeded 50 characters" << endl;
						the_file_mgr.print_current_line();
		     			cin.get();
					    exit(1);
					}
					break;
			case 2: add_char(check);
					break;
			case 3: add_char(check);
					break;
			case 4: add_char(check);
					break;
			case 5: add_char(check);
					break;
			case 6: clear_buffer();
					break;
			case 7: add_char(check);
					break;
			case 8: add_char(check);
					break;
			case 9: add_char(check);
					break;
			case 10:
			case 11: break;
			case 12: if (lexeme_length < 12) add_char(check);
					 else { //ERROR: too long identifier
						cout << "LEXICAL ERROR: Identifier length exceeded 12 characters" << endl;
						the_file_mgr.print_current_line();
					    cin.get();
					    exit(1);
					 }
					 break;
			case 13: if (lexeme_length < 4) add_char(check);
					 else { //ERROR: too long unsigned integer
						 cout << "LEXICAL ERROR: Unsigned integer length exceeded 4 characters" << endl;
						 the_file_mgr.print_current_line();
						 cin.get();
						 exit(1);
					 }
					 break;

//ERROR CASES
			case 20: cout << "LEXICAL ERROR: carriage return reached while processing string" << endl;
					 the_file_mgr.print_current_line();
					 cin.get();
					 exit(1);
			case 21: cout << "LEXICAL ERROR: Invalid symbol " << check << " encountered" << endl;
					 the_file_mgr.print_current_line();
					 cin.get();
					 exit(1);
			case 22: cout << "LEXICAL ERROR: EOF reached while processing string" << endl;
					 the_file_mgr.print_current_line();
					 cin.get();
					 exit(1);
			case 23: cout << "LEXICAL ERROR: EOF reached while processing comment" << endl;
					 the_file_mgr.print_current_line();
					 cin.get();
					 exit(1);
			case 24: cout << "LEXICAL ERROR: Illegal symbol / encountered" << endl;
					 the_file_mgr.print_current_line();
					 cin.get();
					 exit(1);

//FINAL CASES
			case 100: token.update(STRING_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 101: add_char(check);
					  token.update(RELOP_T, NOTEQUAL_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 102: add_char(check);
					  token.update(RELOP_T, EQUAL_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 103: token.update(ASSIGN_T, NONE_ST, lexeme_buffer);
					  the_file_mgr.pushback();
					  clear_buffer();
					  return;
			case 104: add_char(check);
					  token.update(RELOP_T, LESSEQ_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 105: token.update(RELOP_T, LESS_ST, lexeme_buffer);
					  the_file_mgr.pushback();
					  clear_buffer();
					  return;
			case 106: add_char(check);
					  token.update(INSERT_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 107: add_char(check);
					  token.update(SWAP_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 108: add_char(check);
					  token.update(RELOP_T, GREATEQ_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 109: token.update(RELOP_T, GREAT_ST, lexeme_buffer);
					  the_file_mgr.pushback();
					  clear_buffer();
					  return;
			case 110: add_char(check);
					  token.update(EXTRACT_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 111: token.update(EOF_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 112: clear_buffer();
					  state = 0;
					  break;
			case 113: check_keywords(token); //will call token.update
					  the_file_mgr.pushback();
					  clear_buffer();
					  return;
			case 114: token.update(DIGIT_T, NONE_ST, lexeme_buffer);
					  the_file_mgr.pushback();
					  clear_buffer();
					  return;
			case 115: add_char(check);
				      token.update(COMMA_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 116: add_char(check);
				      token.update(SEMICOLON_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 117: add_char(check);
				      token.update(ADDOP_T, MINUS_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 118: add_char(check);
				      token.update(ADDOP_T, PLUS_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 119: add_char(check);
				      token.update(MULOP_T, MULTIPLY_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 120: add_char(check);
				      token.update(CLOSING_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 121: add_char(check);
				      token.update(OPENING_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 122: add_char(check);
				      token.update(REFERENCE_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 123: add_char(check);
				      token.update(PERIOD_T, NONE_ST, lexeme_buffer);
					  clear_buffer();
					  return;
			case 124: add_char(check);
				      check_compiler_dir();
					  clear_buffer();
					  state = 0;
					  break;
			case 125: clear_buffer();
					  state = 0;
					  break;
			case 126: token.update(EOF_T, NONE_ST, lexeme_buffer);
					  return;
		}
	} while (1);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=add_char
void scanner_class::add_char(char to_add)
//Precondition:  A character has been pulled from the file
//Postcondition: The character is added to the lexeme buffer
{
	lexeme_buffer[lexeme_length++] = to_add;
	lexeme_buffer[lexeme_length] = '\0';
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=check_keywords
void scanner_class::check_keywords(token_class &token)
//Precondition:  None.
//Postcondition: Token is updated to be the relevant YASL
//				 keyword, or an identifier if the lexeme
//				 does not match YASL keywords
{
	int type = IDENTIFIER_T;
	int subtype = NONE_ST;
	if (stricmp(lexeme_buffer,"program")==0) type = PROGRAM_T;
	else if (stricmp(lexeme_buffer,"function")==0) type = FUNCTION_T;
	else if (stricmp(lexeme_buffer,"begin")==0) type = BEGIN_T;
	else if (stricmp(lexeme_buffer,"end")==0) type = END_T;
	else if (stricmp(lexeme_buffer,"if")==0) type = IF_T;
	else if (stricmp(lexeme_buffer,"then")==0) type = THEN_T;
	else if (stricmp(lexeme_buffer,"else")==0) type = ELSE_T;
	else if (stricmp(lexeme_buffer,"while")==0) type = WHILE_T;
	else if (stricmp(lexeme_buffer,"do")==0) type = DO_T;
	else if (stricmp(lexeme_buffer,"cout")==0) type = COUT_T;
	else if (stricmp(lexeme_buffer,"cin")==0) type = CIN_T;
	else if (stricmp(lexeme_buffer,"endl")==0) type = ENDL_T;
	else if (stricmp(lexeme_buffer,"or")==0) {type = ADDOP_T; subtype = OR_ST;}
	else if (stricmp(lexeme_buffer,"and")==0) {type = MULOP_T; subtype = AND_ST;}
	else if (stricmp(lexeme_buffer,"div")==0) {type = MULOP_T; subtype = DIV_ST;}
	else if (stricmp(lexeme_buffer,"mod")==0) {type = MULOP_T; subtype = MOD_ST;}
	else if (stricmp(lexeme_buffer,"int")==0) type = INT_T;
	else if (stricmp(lexeme_buffer,"boolean")==0) type = BOOLEAN_T;
	else if (stricmp(lexeme_buffer,"true")==0) type = TRUE_T;
	else if (stricmp(lexeme_buffer,"false")==0) type = FALSE_T;
	token.update(type, subtype, lexeme_buffer);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=check_compiler_dir
void scanner_class::check_compiler_dir()
//Precondition:  None.
//Postcondition: Compiler reacts to valid compiler directive,
//				 or prints warning message if invalid.
{
	if (lexeme_buffer[2] == 'p') {
		if (lexeme_buffer[3] == '+') the_file_mgr.set_print_status(true);
		else if (lexeme_buffer[3] == '-') the_file_mgr.set_print_status(false);
	}
	else if (lexeme_buffer[2] == 'e') {
		if (lexeme_buffer[3] == '+') expression_debugging = true;
		else if (lexeme_buffer[3] == '-') expression_debugging = false;
	}
	else if (lexeme_buffer[2] == 's' && lexeme_buffer[3] == '+') {
		the_table.print_symbol_table();
	}
	else { //WARNING: Invalid compiler directive
		cout << "LEXWARNING: Invalid compiler directive" << endl;
		the_file_mgr.print_current_line();
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=clear_buffer
void scanner_class::clear_buffer()
//Precondition:  None.
//Postcondition: Lexeme buffer is cleared and reset
{
	lexeme_buffer[0] = '\0';
	lexeme_length = 0;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=close_file
void scanner_class::close_file()
//Precondition:  The file belonging to the object owning this routine has
//               been opened.
//Postcondition: The file belonging to the object owning this routine has
//               been closed.
{
	the_file_mgr.close_source_program();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=is_expression_debugging
bool scanner_class::is_expression_debugging()
//Precondition:  None.
//Postcondition: Returns the status of expression debugging
{
	return expression_debugging;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print_current_line
void scanner_class::print_current_line()
//Precondition:  None.
//Postcondition: Calls print_current_line() of Filemanager
{
	the_file_mgr.print_current_line();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_print_lines_compiled
void scanner_class::s_print_lines_compiled()
//Precondition:  None.
//Postcondition: Prints the number of lines compiled
{
	cout << "Compiled " << the_file_mgr.num_lines_processed() << " lines of code" << endl;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_table_add_level
void scanner_class::s_table_add_level(char s_level_name[])
//Precondition:  None.
//Postcondition: Calls table_add_level
{
	the_table.table_add_level(s_level_name);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_table_del_level
void scanner_class::s_table_del_level()
//Precondition:  None.
//Postcondition: Calls table_del_level
{
	the_table.table_del_level();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_table_add_entry
void scanner_class::s_table_add_entry(char s_name[], int s_kind, int s_type)
//Precondition:  None.
//Postcondition: Calls table_add_entry
{
	bool duplicate = the_table.check_duplicate_id(s_name);
	if (duplicate) { //ERROR duplicate identifier
		cout << "SYNTAXERROR: Duplicate identifier <" << s_name << "> found" << endl;
		the_file_mgr.print_current_line();
		cin.get();
		exit(1);
	}
	the_table.table_add_entry(s_name, s_kind, s_type);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_table_lookup
table_node* scanner_class::s_table_lookup(char s_compare[])
//Precondition:  None.
//Postcondition: Calls table_lookup
{
	return the_table.table_lookup(s_compare);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_get_temp_ptr
table_node* scanner_class::s_get_temp_ptr()
//Precondition:  None.
//Postcondition: Calls get_temp_ptr
{
	return the_table.get_temp_ptr();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_delete_temps
void scanner_class::s_delete_temps()
//Precondition:  None.
//Postcondition: Calls delete_temps
{
	the_table.delete_temps();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_delete_special_temps
void scanner_class::s_delete_special_temps()
//Precondition:  None.
//Postcondition: Calls delete_special_temps
{
	the_table.delete_special_temps();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_insert_func
void scanner_class::s_insert_func(char check[],int num)
//Precondition:  None.
//Postcondition: Calls insert_func
{
	the_table.insert_func(check,num);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_get_param_type
bool scanner_class::s_get_param(char func_check[],int check_offset,int &kind, int &type)
//Precondition:  None.
//Postcondition: Calls get_param
{
	return the_table.get_param(func_check,check_offset,kind,type);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_reset_offset
void scanner_class::s_reset_offset()
//Precondition:  None.
//Postcondition: Calls reset_offset
{
	the_table.reset_offset();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_insert_num_params
void scanner_class::s_insert_num_params(int p)
//Precondition:  None.
//Postcondition: Calls insert_num_params
{
	the_table.insert_num_params(p);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=s_get_top_nesting_level
int scanner_class::s_get_top_nesting_level()
//Precondition:  None.
//Postcondition: Calls get_top_nesting_level
{
	return the_table.get_top_nesting_level();
}
///////////////////////////////////////////////////

///////////////////////////////////////////////////
////          NON-MEMBER FUNCTIONS             ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=get_next_state
int get_next_state(int row, char ch)
//Precondition:  None.
//Postcondition: The next state to visit in the FSA
//				 while compiling the file is returned
{						   	 
	                          //  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
	                          //  l,  d,  _,  =,  <,  >,  +,  -,  *,  {,  },  (,  ),  ',  ;,  ,,  .,  $,  &,  /, ws, cr,EOF,other
	static int table[20][24] = { 12, 13, 21,  2,  3,  4,118,117,119,  5, 21,121,120,  1,116,115,123, 21,122, 10,  0,  0,126, 21, //0
		                          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,100,  1,  1,  1,  1,  1,  1,  1, 20, 22,  1, //1
					  		    103,103,103,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103, //2
							    105,105,105,104,106,101,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105, //3
							    109,109,109,108,107,110,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109, //4
							      6,  6,  6,  6,  6,  6,  6,  6,  6,  6,125,  6,  6,  6,  6,  6,  6,  7,  6,  6,  6,  6, 23,  6, //5
							 	  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,125,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 23,  6, //6
							      8,  6,  6,  6,  6,  6,  6,  6,  6,  6,125,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 23,  6, //7
								  6,  6,  6,  6,  6,  6,  9,  9,  6,  6,125,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 23,  6, //8
								  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,124,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 23,  6, //9
							     24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 11, 24, 24, 24, 24, //10
							     11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,112,111, 11, //11
							     12, 12, 12,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113, //12
							    114, 13,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114, //13
								 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //14
								 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //15
								 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //16
								 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //17
								 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //18
								 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //19
							   };
	int column = -1;
	if (isalpha(ch)) column = 0;		//catch all for letters
	else if (isdigit(ch)) column = 1;	//catch all for digits
	else if (ch == '_') column = 2;
	else if (ch == '=') column = 3;
	else if (ch == '<') column = 4;
	else if (ch == '>') column = 5;
	else if (ch == '+') column = 6;
	else if (ch == '-') column = 7;
	else if (ch == '*') column = 8;
	else if (ch == '{') column = 9;
	else if (ch == '}') column = 10;
	else if (ch == '(') column = 11;
	else if (ch == ')') column = 12;
	else if (ch == '\'') column = 13;
	else if (ch == ';') column = 14;
	else if (ch == ',') column = 15;
	else if (ch == '.') column = 16;
	else if (ch == '$') column = 17;
	else if (ch == '&') column = 18;
	else if (ch == '/') column = 19;
	else if (ch == '\n') column = 21; //SWITCHED 20 and 21 because isspace() finds carriage returns too
	else if (isspace(ch)) column = 20;
	else if (ch == EOF) column = 22;
	else column = 23;
	
	return (table[row][column]);
}
///////////////////////////////////////////////////
