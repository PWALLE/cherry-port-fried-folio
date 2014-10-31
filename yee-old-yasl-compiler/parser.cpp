//Paul Elliott, Revised 6 December 2009
//This file contains method implementations for the
//parser class, as well as non-member functions
//precedence and valid_expression
//Modified: Took out reading token upon entering parse_expression,
//			made ; or do keyword valid ending tokens, added do keyword
//			to precedence function
//Added: Implementations for parse_program, print_lines_compiled,
//		 statement, and non-member first_S
//Modified: Got rid of first set functions
//Added: Implementations for statement grammar functions
//Added: Implementations for the program grammar functions
//Added: parse_error implementation
//Modified: Incorporated symbol table to parsers, now builds
//			symbol table and checks for 1. Duplicate IDs
//			2. Undeclared IDs 3. Incorrect # Parameters
//Modified: Added assembler output for 1. cin; 2. cout << STR-Const
//			3. cout << endl, and for beginning and ending main program.
//ADDED:	Added functionality for printing assembly language for ASLSOYASL,
//			including functions get_next_temp and get_next_skip, and evaluate_expression
//Finished: assembly language output.

//The class header is found in parser.h

#include "stdafx.h"  // Required for visual studio
#include "parser.h"


///////////////////////////////////////////////////
////         DEFINITION FOR PARSER             ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=parser_class()
parser_class::parser_class()
//Precondition:  None.
//Postcondition: The parser is initialized, and prepares the outfile to
//				 receive assembly language.
{
	outfile.open("out.pal", ios::out);
	count_params = -1;
	count_globals = 0;
	isWhile = false;
	isIf = false;
	isFuncCall = false;
	paramType = -1;
	paramKind = -1;
	nesting = 0;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=parse_program
void parser_class::parse_program()
//Precondition:  None.
//Postcondition: Loads the first token and starts the parse
{
	the_scanner.get_next_token(the_token);
	
	program();
	outfile.close(); //ASSEMBLER: CLOSE OUTFILE
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=program
void parser_class::program()
//Precondition:  None.
//Postcondition: Parses the program, returning quietly if valid and displaying an error if not
{
	//<program> -> program id ; <block> .
	if (the_token.get_type() == PROGRAM_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T) {
			the_scanner.get_next_token(the_token);
			if (the_token.get_type() == SEMICOLON_T) {
				//ASSEMBLER: BEGIN MAIN PROGRAM
				outfile << "$main movw SP R0" << endl;
				the_scanner.get_next_token(the_token);
				if (the_token.get_type() == INT_T ||
					the_token.get_type() == BOOLEAN_T ||
					the_token.get_type() == FUNCTION_T ||
					the_token.get_type() == BEGIN_T) { //guard block
					//ADD MAIN LEVEL TO SYMBOL TABLE
					the_scanner.s_table_add_level("main");
					nesting++;
					block();
					if (the_token.get_type() == PERIOD_T) {
						//ASSEMBLER: END MAIN PROGRAM
						outfile << "end" << endl;
						the_scanner.get_next_token(the_token);
						return;
					}
					else { //ERROR expected PERIOD_T
						parse_error(the_token, ".");
					}
				}
				else { //ERROR expected block
					parse_error(the_token, "INT BOOLEAN FUNCTION BEGIN");
				}
			}
			else { //ERROR expected SEMICOLON_T
				parse_error(the_token, ";");
			}
		}
		else { //ERROR expected IDENTIFIER_T
			parse_error(the_token, "IDENTIFIER");
		}
	}
	else { //ERROR expected PROGRAM_T
		parse_error(the_token, "PROGRAM");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=block
void parser_class::block()
//Precondition:  None.
//Postcondition: Parses the block, returning quietly if valid and displaying an error if not
{	
	count_globals = 0;
	char temp_begin[10] = "";
	char ar_temp[10] = "";
	// <block> -> <var_decs> <func_decs> <prog_body>
	if (the_token.get_type() == INT_T ||
	    the_token.get_type() == BOOLEAN_T ||
	    the_token.get_type() == FUNCTION_T ||
	    the_token.get_type() == BEGIN_T) { //guard var_decs
		var_decs();
		char vars[5] = "";
		sprintf(vars,"#%d",count_globals*4);
		print_instruction("addw",NULL,vars,NULL,"SP"); //ASSEMBLER: Make space for globals on PAL stack
		get_next_begin(temp_begin);
		print_instruction("jmp",NULL,temp_begin,NULL,""); //ASSEMBLER: Jump to begin
		if (the_token.get_type() == FUNCTION_T ||
		    the_token.get_type() == BEGIN_T) { //guard func_decs
			func_decs();
			if (the_token.get_type() == BEGIN_T) { //guard prog_body
				if (the_scanner.s_get_top_nesting_level() == 0) { //ASSEMBLER: Main runthrough, set aside space for display and update main's display
					outfile << "$display #" << nesting*4 << endl;
					outfile << temp_begin << " movw R0 +0$display" << endl;
				}
				else { //ASSEMBLER: function runthrough, save old display value and update
					get_next_special_temp(ar_temp);
					the_scanner.s_table_add_entry(ar_temp,SUPER_SPECIAL_TEMP_KIND,SPECIAL_TEMP_TYPE);
					pointer = the_scanner.s_table_lookup(ar_temp);
					outfile << "movw +" << the_scanner.s_get_top_nesting_level()*4 << "$display +" << pointer->node_offset*4 << "@FP" << endl;
					outfile << temp_begin << " movw FP +" << the_scanner.s_get_top_nesting_level()*4 << "$display" << endl;
				}
				prog_body();
				//ASSEMBLER restore display
				if (the_scanner.s_get_top_nesting_level() > 0) {
					pointer = the_scanner.s_table_lookup(ar_temp);
					char restored[12] = "";
					int offset = pointer->node_nesting_level*4;
					sprintf(restored,"+%d$display",offset);
					print_instruction("movw",pointer,"",NULL,restored);
				}
				return;
			}
			else { //ERROR expected prog_body
				parse_error(the_token, "BEGIN");
			}
		}
		else { //ERROR expected func_decs or prog_body
			parse_error(the_token, "FUNCTION BEGIN");
		}
	}
	else { //ERROR expected block
		parse_error(the_token, "INT BOOLEAN FUNCTION BEGIN");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=var_decs
void parser_class::var_decs()
//Precondition:  None.
//Postcondition: Parses the var_decs, returning quietly if valid and displaying an error if not
{
	
	//<var_decs> -> <type> <ident_list> ; <var_decs>
	if (the_token.get_type() == INT_T ||
		the_token.get_type() == BOOLEAN_T) { //guard type
		type();
		if (the_token.get_type() == IDENTIFIER_T) { //guard ident_list
			ident_list();
			if (the_token.get_type() == SEMICOLON_T) {
				the_scanner.get_next_token(the_token);
				if (the_token.get_type() == INT_T ||
					the_token.get_type() == BOOLEAN_T ||
					the_token.get_type() == FUNCTION_T ||
					the_token.get_type() == BEGIN_T) { //guard var_decs
						var_decs();
						return;
				}
				else { //ERROR expected var_decs
					parse_error(the_token, "INT BOOLEAN FUNCTION BEGIN");
				}
			}
			else { //ERROR expected SEMICOLON_T
			parse_error(the_token, ";");
			}
		}
		else { //ERROR expected ident_list
			parse_error(the_token, "IDENTIFIER");
		}
	}
	//<var_decs> -> <e>
	else if (the_token.get_type() == FUNCTION_T ||
			 the_token.get_type() == BEGIN_T) { //follow set
			 return;
	}
	else { //ERROR expected var_decs
		parse_error(the_token, "INT BOOLEAN FUNCTION BEGIN");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=type
void parser_class::type()
//Precondition:  None.
//Postcondition: Parses the type, returning quietly if valid and displaying an error if not
{
	//<type> -> int
	if (the_token.get_type() == INT_T) {
		global_type = INT_TYPE;
		the_scanner.get_next_token(the_token);
		return;
	}
	//<type> -> boolean
	else if (the_token.get_type() == BOOLEAN_T) {
		global_type = BOOLEAN_TYPE;
		the_scanner.get_next_token(the_token);
		return;
	}
	else { //ERROR expected INT_T BOOLEAN_T
		parse_error(the_token, "INT BOOLEAN");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=ident_list
void parser_class::ident_list()
//Precondition:  None.
//Postcondition: Parses the ident_list, returning quietly if valid and displaying an error if not
{
	//<ident_list> -> id <identifier_tail>
	if (the_token.get_type() == IDENTIFIER_T) {
		//ADD VARIABLE IDENTIFIER TO CURRENT SCOPE LEVEL
		the_scanner.s_table_add_entry(the_token.the_lexeme,VAR_ID,global_type);
		count_globals++; //Global id found
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == COMMA_T ||
			the_token.get_type() == SEMICOLON_T) { //guard identifier_tail
				identifier_tail();
				return;
		}
		else { //ERROR expected COMMA_T SEMICOLON_T
			parse_error(the_token, ", ;");
		}
	}
	else { //ERROR expected IDENTIFIER_T
		parse_error(the_token, "IDENTIFIER");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=identifier_tail
void parser_class::identifier_tail()
//Precondition:  None.
//Postcondition: Parses the identifier_tail, returning quietly if valid and displaying an error if not
{
	//<identifier_tail> -> , <ident_list>
	if (the_token.get_type() == COMMA_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T) { //guard ident_list
			ident_list();
			return;
		}
		else { //ERROR expected ident_list
			parse_error(the_token, "IDENTIFIER");
		}
	}
	//<identifier_tail> -> <e>
	else if (the_token.get_type() == SEMICOLON_T) { //follow set
		return;
	}
	else { //ERROR expected identifier_tail
		parse_error(the_token, ", ;");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=func_decs
void parser_class::func_decs()
//Precondition:  None.
//Postcondition: Parses the func_decs, returning quietly if valid and displaying an error if not
{
	static int func_num = 0;
	//<func_decs> -> function identifier <func_dec_tail> ; <block> ; <func_decs>
	if (the_token.get_type() == FUNCTION_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T) {
			//ADD FUNCTION IDENTIFIER TO CURRENT SCOPE LEVEL
			the_scanner.s_table_add_entry(the_token.the_lexeme,FUNC_ID,FUNC_ID_TYPE);
			the_scanner.s_insert_func(the_token.the_lexeme,func_num);
			outfile << "$func" << func_num << " movw R0 R0" << endl; //ASSEMBLER: func jump
			func_num++;
			//ADD NEXT SCOPE LEVEL TO SYMBOL TABLE
			the_scanner.s_table_add_level(the_token.the_lexeme);
			if ((the_scanner.s_get_top_nesting_level()+1) > nesting) nesting = the_scanner.s_get_top_nesting_level()+1;
			the_scanner.get_next_token(the_token);
			if (the_token.get_type() == OPENING_T ||
				the_token.get_type() == SEMICOLON_T) { //guard func_dec_tail
				func_dec_tail();
				if (the_token.get_type() == SEMICOLON_T) {
					the_scanner.get_next_token(the_token);
					if (the_token.get_type() == INT_T ||
						the_token.get_type() == BOOLEAN_T ||
						the_token.get_type() == FUNCTION_T ||
						the_token.get_type() == BEGIN_T) { //guard block
						block();
						if (the_token.get_type() == SEMICOLON_T) {
							//EXITED SCOPE LEVEL, POP SYMBOL TABLE
							the_scanner.s_table_del_level();
							print_instruction("movw",NULL,"FP",NULL,"SP"); //ASSEMBLER: bug fix
							outfile << "ret" << endl;
							the_scanner.get_next_token(the_token);
							if (the_token.get_type() == FUNCTION_T ||
								the_token.get_type() == BEGIN_T) { //guard func_decs
								func_decs();
							}
							else { //ERROR expected func_decs
								parse_error(the_token, "FUNCTION BEGIN");
							}
						}
						else { //ERROR expected SEMICOLON_T
							parse_error(the_token, ";");
						}
					}
					else { //ERROR expected block
						parse_error(the_token, "INT BOOLEAN FUNCTION BEGIN");
					}
				}
				else { //ERROR expected SEMICOLON_T
					parse_error(the_token, ";");
				}
			}
			else { //ERROR expected func_dec_tail
				parse_error(the_token, "( ;");
			}
		}
		else { //ERROR expected IDENTIFIER_T
			parse_error(the_token, "IDENTIFIER");
		}
	}
	//<func_decs> -> <e>
	else if (the_token.get_type() == BEGIN_T) { //follow set
		return;
	}
	else { //ERROR expected func_decs
		parse_error(the_token, "FUNCTION BEGIN");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=func_dec_tail
void parser_class::func_dec_tail()
//Precondition:  None.
//Postcondition: Parses the func_dec_tail, returning quietly if valid and displaying an error if not
{
	//<func_dec_tail> -> ( <param_list> )
	if (the_token.get_type() == OPENING_T) {
		count_params = 0;
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == INT_T ||
			the_token.get_type() == BOOLEAN_T) { //guard param_list
			param_list();
			if (the_token.get_type() == CLOSING_T) {
				the_scanner.s_insert_num_params(count_params); //insert number of params for each param symbol
				count_params = -1;
				the_scanner.s_reset_offset(); //to fix PAL stack
				the_scanner.get_next_token(the_token);
				return;
			}
			else { //ERROR expected CLOSING_T
			parse_error(the_token, ")");
			}
		}
		else { //ERROR expected param_list
			parse_error(the_token, "INT BOOLEAN");
		}
	}
	//<func_dec_tail> -> <e>
	else if (the_token.get_type() == SEMICOLON_T) { //follow set
		return;
	}
	else { //ERROR expected func_decs
		parse_error(the_token, "( ;");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=param_list
void parser_class::param_list()
//Precondition:  None.
//Postcondition: Parses the param_list, returning quietly if valid and displaying an error if not
{
	//<param_list> -> <type> <type_tail>
	if (the_token.get_type() == INT_T ||
		the_token.get_type() == BOOLEAN_T) { //guard type
		type();
		if (the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == REFERENCE_T) { //guard  type_tail
			type_tail();
			return;
		}
		else { //ERROR expected type_tail
			parse_error(the_token, "IDENTIFIER &");
		}
	}
	else { //ERROR expected param_list
		parse_error(the_token, "INT BOOLEAN");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=type_tail
void parser_class::type_tail()
//Precondition:  None.
//Postcondition: Parses the type_tail, returning quietly if valid and displaying an error if not
{
	//<type_tail> -> id <ident_tail>
	if (the_token.get_type() == IDENTIFIER_T) {
		count_params++;
		//ADD VALUE PARAMETER TO CURRENT SCOPE LEVEL
		the_scanner.s_table_add_entry(the_token.the_lexeme, VALUE_PARAM, global_type);
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == COMMA_T ||
			the_token.get_type() == CLOSING_T) { //guard ident_tail
			ident_tail();
			return;
		}
		else { //ERROR expected ident_tail
			parse_error(the_token, ", )");
		}
	}
	//<type_tail> -> & id <ident_tail>
	else if (the_token.get_type() == REFERENCE_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T) {
			count_params++;
			//ADD REFERENCE PARAMETER TO CURRENT SCOPE LEVEL
			the_scanner.s_table_add_entry(the_token.the_lexeme, REF_PARAM, global_type);
			the_scanner.get_next_token(the_token);
			if (the_token.get_type() == COMMA_T ||
				the_token.get_type() == CLOSING_T) { //guard ident_tail
				ident_tail();
				return;
			}
			else { //ERROR expected ident_tail
				parse_error(the_token, ", )");
			}
		}
		else { //ERROR expected IDENTIFIER
			parse_error(the_token, "IDENTIFIER");
		}
	}
	else { //ERROR expected type_tail
		parse_error(the_token, "IDENTIFIER &");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=ident_tail
void parser_class::ident_tail()
//Precondition:  None.
//Postcondition: Parses the ident_tail, returning quietly if valid and displaying an error if not
{
	//<ident_tail> -> , <param_list>
	if (the_token.get_type() == COMMA_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == INT_T ||
			the_token.get_type() == BOOLEAN_T) { //guard param_list
			param_list();
			return;
		}
		else { //ERROR expected param_list
			parse_error(the_token, "INT BOOLEAN");
		}
	}
	//<ident_tail> -> <e>
	else if (the_token.get_type() == CLOSING_T) { //follow set
		return;
	}
	else { //ERROR expected ident_tail
		parse_error(the_token, ", )");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=prog_body
void parser_class::prog_body()
//Precondition:  None.
//Postcondition: Parses the prog_body, returning quietly if valid and displaying an error if not
{
	//<prog_body> -> begin <follow_begin>
	if (the_token.get_type() == BEGIN_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == END_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == IF_T ||
			the_token.get_type() == WHILE_T ||
			the_token.get_type() == BEGIN_T ||
			the_token.get_type() == CIN_T ||
			the_token.get_type() == COUT_T) { //guard follow_begin
			follow_begin();
			return;
		}
		else { //ERROR expected follow_begin
			parse_error(the_token, "END IDENTIFIER IF WHILE BEGIN CIN COUT");
		}
	}
	else { //ERROR expected prog_body
		parse_error(the_token, "BEGIN");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=statement
void parser_class::statement()
//Precondition:  None.
//Postcondition: Parses the statement, returning quietly if valid and displaying an error if not
{
	char skipif[8] = "";
	char skipelse[8] = "";
	char whiletop[8] = "";
	char whileafter[8] = "";
	//<statement> -> while <expression> do <statement>
	if (the_token.get_type() == WHILE_T) {
		isWhile = true;
		get_next_while(whiletop,whileafter);
		the_scanner.get_next_token(the_token);
		outfile << whiletop << " movw R0 R0" << endl;
		if (the_token.get_type() == OPENING_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == DIGIT_T ||
			the_token.get_type() == TRUE_T ||
			the_token.get_type() == FALSE_T) { //guard parse_expression
			parse_expression();
			isWhile = false;
			//ASSEMBLER: top of while code
			print_instruction("cmpw",NULL,"#0",NULL,"-4@SP");
			print_instruction("subw",NULL,"#4",NULL,"SP");
			print_instruction("beq",NULL,whileafter,NULL,"");
			if (the_token.get_type() == DO_T) {
				the_scanner.get_next_token(the_token);
				if (the_token.get_type() == IDENTIFIER_T ||
					the_token.get_type() == IF_T ||
					the_token.get_type() == WHILE_T ||
					the_token.get_type() == BEGIN_T ||
					the_token.get_type() == CIN_T ||
					the_token.get_type() == COUT_T) { //guard statement
					statement();
					//ASSEMBLER: bottom of while code
					print_instruction("jmp",NULL,whiletop,NULL,"");
					outfile << whileafter << " movw R0 R0" << endl;
					return;
				}
				else { //ERROR expected statement
				parse_error(the_token, "IDENTIFIER IF WHILE BEGIN CIN COUT");
				}
			}
			else { //ERROR expected DO_T
				parse_error(the_token, "DO");
			}
		}
		else { //ERROR expected parse_expression
			parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE");
		}
	}
	//<statement> -> if <expression> then <statement> <follow_if>
	else if (the_token.get_type() == IF_T) {
		isIf = true;
		get_next_if(skipif,skipelse);
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == OPENING_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == DIGIT_T ||
			the_token.get_type() == TRUE_T ||
			the_token.get_type() == FALSE_T) { //guard parse_expression
			parse_expression();
			isIf = false;
			if (the_token.get_type() == THEN_T) {
				//ASSEMBLER: top of if code
				print_instruction("cmpw",NULL,"#0",NULL,"-4@SP");
				print_instruction("subw",NULL,"#4",NULL,"SP");
				print_instruction("beq",NULL,skipif,NULL,"");
				the_scanner.get_next_token(the_token);
				if (the_token.get_type() == IDENTIFIER_T ||
					the_token.get_type() == IF_T ||
					the_token.get_type() == WHILE_T ||
					the_token.get_type() == BEGIN_T ||
					the_token.get_type() == CIN_T ||
					the_token.get_type() == COUT_T) { //guard statement
					statement();
					if (the_token.get_type() == ELSE_T ||
						the_token.get_type() == SEMICOLON_T ||
						the_token.get_type() == END_T) { //guard follow_if 
						follow_if(skipif,skipelse);
						return;
					}
					else { //ERROR expected follow_if
						parse_error(the_token, "ELSE ; END");
					}
				}
				else { //ERROR expected statement
					parse_error(the_token, "IDENTIFIER IF WHILE BEGIN CIN COUT");
				}
			}
			else { //ERROR expected THEN_T
				parse_error(the_token, "THEN");
			}
		}
		else { //ERROR expected parse_expression
			parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE");
		}
	}
	//<statement> -> identifier <follow_id>
	else if (the_token.get_type() == IDENTIFIER_T) {
		//CHECK FOR UNDECLARED IDENTIFIER
		pointer = the_scanner.s_table_lookup(the_token.the_lexeme);
		if (pointer == NULL) { //ERROR undeclared identifier
			undeclared_id(the_token);
		}
		//IF IDENTIFIER IS A FUNCTION, COUNT PARAMETERS
		if (pointer->kind == FUNC_ID) {
			count_params = 0;
			table_node* param_pointer = pointer->parameters;
			while (param_pointer != NULL) {
				count_params++;
				param_pointer = param_pointer->next_node;
			}
		}
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == ASSIGN_T ||
			the_token.get_type() == SWAP_T ||
			the_token.get_type() == OPENING_T ||
			the_token.get_type() == ELSE_T ||
			the_token.get_type() == SEMICOLON_T ||
			the_token.get_type() == END_T) { //guard follow_id
				follow_id(pointer->node_lexeme);
				return;
		}
		else { //ERROR expected follow_id
			parse_error(the_token, "= >< ( ELSE ; END");
		}
	}
	//<statement> -> cout <follow_cout>
	else if (the_token.get_type() == COUT_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == INSERT_T) { //guard follow_cout
			follow_cout();
			return;
		}
		else { //ERROR expected follow_cout
			parse_error(the_token, "<<");
		}
	}
	//<statement> -> cin <follow_cin>
	else if (the_token.get_type() == CIN_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == EXTRACT_T ||
			the_token.get_type() == ELSE_T ||
			the_token.get_type() == SEMICOLON_T||
			the_token.get_type() == END_T) {
			follow_cin();
			return;
		}
		else { //ERROR expected follow_cin
			parse_error(the_token, "EXTRACT ELSE ; END");
		}
	}
	//<statement> -> begin <follow_begin>
	else if (the_token.get_type() == BEGIN_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == END_T ||
			the_token.get_type() == WHILE_T ||
			the_token.get_type() == IF_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == COUT_T ||
			the_token.get_type() == CIN_T ||
			the_token.get_type() == BEGIN_T) { //guard follow_begin
			follow_begin();
			return;
		}
		else { //ERROR expected follow_begin
			parse_error(the_token, "END IDENTIFIER IF WHILE BEGIN CIN COUT");
		}
	}
	else { //ERROR expected statement
		parse_error(the_token, "IDENTIFIER WHILE IF BEGIN CIN COUT");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=follow_begin
void parser_class::follow_begin()
//Precondition:  None.
//Postcondition: Parses follow_begin, returning quietly if valid and displaying an error if not
{
	//<follow_begin> -> end
	if (the_token.get_type() == END_T) {
		the_scanner.get_next_token(the_token);
		return;
	}
	//<follow_begin> -> <statement> <statement_tail> end
	else if (the_token.get_type() == IDENTIFIER_T ||
			 the_token.get_type() == IF_T ||
			 the_token.get_type() == WHILE_T ||
			 the_token.get_type() == BEGIN_T ||
			 the_token.get_type() == CIN_T ||
			 the_token.get_type() == COUT_T) { //guard statement
			 statement();
			 if (the_token.get_type() == SEMICOLON_T ||
				 the_token.get_type() == END_T) { //guard statement_tail
				 statement_tail();
				 if (the_token.get_type() == END_T) {
					 the_scanner.get_next_token(the_token);
					 return;
				 }
				 else { //ERROR expected END_T
					parse_error(the_token, "END");
				 }
			 }
			 else { //ERROR expected statement_tail
				 parse_error(the_token, "; END");
			 }
	}
	else { //ERROR expected follow_begin
		parse_error(the_token, "END IDENTIFIER IF WHILE BEGIN CIN COUT");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=statement_tail
void parser_class::statement_tail()
//Precondition:  None.
//Postcondition: Parses statement_tail, returning quietly if valid and displaying an error if not
{
	//<statement_tail> -> ; <statement> <statement_tail>
	if (the_token.get_type() == SEMICOLON_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == IF_T ||
			the_token.get_type() == WHILE_T ||
			the_token.get_type() == BEGIN_T ||
			the_token.get_type() == CIN_T ||
			the_token.get_type() == COUT_T) { //guard statement
			statement();
			if (the_token.get_type() == SEMICOLON_T ||
				the_token.get_type() == END_T) { //guard statement_tail
				statement_tail();
				return;
			}
			else { //ERROR expected statement_tail
				parse_error(the_token, "; END");
			}
		}
		else { //ERROR expected statement
			parse_error(the_token, "IDENTIFIER IF WHILE BEGIN CIN COUT");
		}
	}
	//<statement_tail> -> <e>
	else if (the_token.get_type() == END_T) { //follow set
		return;
	}
	else { //ERROR expected statement_tail
		parse_error(the_token, "SEMICOLON END");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=follow_if
void parser_class::follow_if(char if_label[], char else_label[])
//Precondition:  None.
//Postcondition: Parses follow_if, returning quietly if valid and displaying an error if not
{
	//<follow_if> -> else <statement>
	if (the_token.get_type() == ELSE_T) {
		//ASSEMBLER: else code
		print_instruction("jmp",NULL,else_label,NULL,"");
		outfile << if_label << " movw R0 R0" << endl;
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == IF_T ||
			the_token.get_type() == WHILE_T ||
			the_token.get_type() == BEGIN_T ||
			the_token.get_type() == CIN_T ||
			the_token.get_type() == COUT_T) { //guard statement
			statement();
			outfile << else_label << " movw R0 R0" << endl;
			return;
		}
		else { //ERROR expected statement
			parse_error(the_token, "IDENTIFIER IF WHILE BEGIN CIN COUT");
		}
	}
	//<follow_if> -> <e>
	else if (the_token.get_type() == SEMICOLON_T ||
			 the_token.get_type() == END_T) { //follow set
	    //ASSEMBLER: end if statement code
	    outfile << if_label << " movw R0 R0" << endl;
		return;
	}
	else { //ERROR expected follow_if
		parse_error(the_token, "ELSE ; END");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=follow_id
void parser_class::follow_id(char assigned_lex[])
//Precondition:  None.
//Postcondition: Parses follow_id, returning quietly if valid and displaying an error if not
{
	char num_params[8] = "";
	char temp_func[8] = "";
	//<follow_id> -> = <expression>
	if (the_token.get_type() == ASSIGN_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == OPENING_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == DIGIT_T ||
			the_token.get_type() == TRUE_T ||
			the_token.get_type() == FALSE_T) { //guard parse_expression
			parse_expression();
			pointer = the_scanner.s_table_lookup(assigned_lex);
			if (pointer->type == FUNC_ID_TYPE) { //ERROR can't assign to function
				cout << "SEMANTICERROR: Invalid use of function id with assignment statement" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			if (pointer->type != expr_type) { //ERROR type mismatch
				cout << "SEMANTICERROR: Type mismatch between [" << assigned_lex << "] (";
				print(pointer->type);
				cout << ") and expression (";
				print(expr_type);
				cout << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			//ASSEMBLER: assign expression value to variable
			print_instruction("movw",NULL,"-4@SP",pointer,"");
			print_instruction("subw",NULL,"#4",NULL,"SP");
			return;
		}
		else { //ERROR expected parse_expression
			parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE");
		}
	}
	//<follow_id> -> >< id
	else if (the_token.get_type() == SWAP_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T) {
			//CHECK FOR UNDECLARED IDENTIFIER
			pointer = the_scanner.s_table_lookup(the_token.the_lexeme);
			if (pointer == NULL) { //ERROR undeclared identifier
				undeclared_id(the_token);
			}
			//cout << "successfully looked up <" << pointer->node_lexeme << ">" << endl;
			table_node* pointer2 = the_scanner.s_table_lookup(assigned_lex);
			if (pointer2->type == FUNC_ID_TYPE || pointer->type == FUNC_ID_TYPE) { //ERROR can't swap function ids
				cout << "SEMANTICERROR: Invalid use of swap with function identifier(s)" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			else if (pointer2->type != pointer->type) { //ERROR type mismatch
				cout << "SEMANTICERROR: Type mismatch between [" << pointer2->node_lexeme << "] (";
				print(pointer2->type);
				cout << ") and [" << pointer->node_lexeme << "] (";
				print(pointer->type);
				cout << ")" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			//ASSEMBLER: SWAP
			print_instruction("movw",pointer2,"",NULL,"@SP");
			print_instruction("movw",pointer,"",pointer2,"");
			print_instruction("movw",NULL,"@SP",pointer,"");
			the_scanner.get_next_token(the_token);
			return;
		}
		else { //ERROR expected IDENTIFIER_T
			parse_error(the_token, "IDENTIFIER");
		}
	}
	//<follow_id> -> ( <expresson> <follow_expression> )
	else if (the_token.get_type() == OPENING_T) {
		params_used = 0; //PREPARE TO COUNT NUMBER OF PASSED PARAMETERS
		isFuncCall = true;
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == OPENING_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == DIGIT_T ||
			the_token.get_type() == TRUE_T ||
			the_token.get_type() == FALSE_T) { //guard parse_expression
			bool success = the_scanner.s_get_param(assigned_lex,params_used,paramKind,paramType);
			if (!success) { //ERROR incorrect number parameters
				cout << "SYNTAXERROR: Incorrect number of parameters for function "
				<< assigned_lex << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			if (paramKind == REF_PARAM) {
				if (the_token.get_type() == IDENTIFIER_T) {
					pointer = the_scanner.s_table_lookup(the_token.the_lexeme);
					if (pointer->type != paramType) { //ERROR type mismatch
						cout << "SEMANTICERROR: Invalid parameter, expected <";
						print(paramType);
						cout << "> but found <";
						print(pointer->type);
						cout << ">" << endl;
						the_scanner.print_current_line();
						cin.get();
						exit(1);
					}
					//ASSEMBLER: put reference parameter on PAL stack
					print_instruction("mova",pointer,"",NULL,"@SP");
					print_instruction("addw",NULL,"#4",NULL,"SP");
					the_scanner.get_next_token(the_token);
				}
				else { //ERROR expected identifier for ref param
					cout << "SEMANTICERROR: Expected <identifier> for reference parameter" << endl;
					the_scanner.print_current_line();
					cin.get();
					exit(1);
				}
			}
			else parse_expression();
			params_used++; //IF FOUND VALID PARAM, INCREMENT PASSED PARAMETERS
			if (the_token.get_type() == COMMA_T ||
				the_token.get_type() == CLOSING_T) { //guard follow_expression
				follow_expression(assigned_lex);
				if (the_token.get_type() == CLOSING_T) {
					if (count_params != params_used) { //ERROR incorrect number parameters
						cout << "SYNTAXERROR: Incorrect number of parameters for function "
						<< assigned_lex << endl;
						the_scanner.print_current_line();
						cin.get();
						exit(1);
					}
					isFuncCall = false;
					//ASSEMBLER: function call
					the_scanner.s_delete_special_temps();
					sprintf(num_params,"#%d",count_params);
					pointer = the_scanner.s_table_lookup(assigned_lex);
					sprintf(temp_func,"$func%d",pointer->node_offset);
					print_instruction("call",NULL,num_params,NULL,temp_func);
					count_params = -1;
					the_scanner.get_next_token(the_token);
					return;
				}
				else { //ERROR expected CLOSING_T
				parse_error(the_token, ")");
				}
			}
			else { //ERROR expected follow_expression
				parse_error(the_token, ", )");
			}
		}
		else { //ERROR expected parse_expression
			parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE");
		}
	}
	//<follow_id> -> <e>
	else if (the_token.get_type() == ELSE_T ||
			 the_token.get_type() == SEMICOLON_T ||
			 the_token.get_type() == END_T) { //follow set
			 if (count_params != 0) { //ERROR incorrect number parameters
				cout << "SYNTAXERROR: Incorrect number of parameters for function "
				<< pointer->node_lexeme << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			 }
			 //ASSEMBLER: function call no parameters
			 sprintf(temp_func,"$func%d",pointer->node_offset);
			 print_instruction("call",NULL,"#0",NULL,temp_func);
			 return;
	}
	else { //ERROR expected follow_id
		parse_error(the_token, "= >< ( ELSE ; END");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=follow_expression
void parser_class::follow_expression(char assigned_lexeme[])
//Precondition:  None.
//Postcondition: Parses follow_expression, returning quietly if valid and displaying an error if not
{
	char temp_special[8] = "";
	//<follow_expression> -> , <expression> <follow_expression>
	if (the_token.get_type() == COMMA_T) {
		//PUT SPECIAL TEMP IN SYMBOL TABLE TO SAVE SPACE FOR PARAMETER
		get_next_special_temp(temp_special);
		the_scanner.s_table_add_entry(temp_special,SPECIAL_TEMP_KIND,SPECIAL_TEMP_TYPE);
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == OPENING_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == DIGIT_T ||
			the_token.get_type() == TRUE_T ||
			the_token.get_type() == FALSE_T) { //guard parse_expression
			bool success = the_scanner.s_get_param(assigned_lexeme,params_used,paramKind,paramType);
			if (!success) { //ERROR incorrect number parameters
				cout << "SYNTAXERROR: Incorrect number of parameters for function "
				<< assigned_lexeme << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			if (paramKind == REF_PARAM) {
				if (the_token.get_type() == IDENTIFIER_T) {
					pointer = the_scanner.s_table_lookup(the_token.the_lexeme);
					if (pointer->type != paramType) { //ERROR type mismatch
						cout << "SEMANTICERROR: Invalid parameter, expected <";
						print(paramType);
						cout << "> but found <";
						print(pointer->type);
						cout << ">" << endl;
						the_scanner.print_current_line();
						cin.get();
						exit(1);
					}
					//ASSEMBLER: put reference parameter on PAL stack
					print_instruction("mova",pointer,"",NULL,"@SP");
					print_instruction("addw",NULL,"#4",NULL,"SP");
					the_scanner.get_next_token(the_token);
				}
				else { //ERROR expected identifier for ref param
					cout << "SEMANTICERROR: Expected <identifier> for reference parameter" << endl;
					the_scanner.print_current_line();
					cin.get();
					exit(1);
				}
			}
			else parse_expression();
			params_used++;
			if (the_token.get_type() == COMMA_T ||
				the_token.get_type() == CLOSING_T) { //guard follow_expression
				follow_expression(assigned_lexeme);
				return;
			}
			else { //ERROR expected follow_expression
				parse_error(the_token, ", ;");
			}
		}
		else { //ERROR expected parse_expression
			parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE");
		}
	}
	//<follow_expression> -> <e>
	else if (the_token.get_type() == CLOSING_T) { //follow set
		return;
	}
	else { //ERROR expected follow_expression
		parse_error(the_token, ", )");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=follow_cin
void parser_class::follow_cin()
//Precondition:  None.
//Postcondition: Parses follow_cin, returning quietly if valid and displaying an error if not
{
	//<follow_cin> -> >> id
	if (the_token.get_type() == EXTRACT_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == IDENTIFIER_T) {
			//CHECK FOR UNDECLARED IDENTIFIER
			pointer = the_scanner.s_table_lookup(the_token.the_lexeme);
			if (pointer == NULL) { //ERROR undeclared identifier
				undeclared_id(the_token);
			}
			else if (pointer->type == FUNC_ID_TYPE || pointer->type == BOOLEAN_TYPE) { //ERROR cin >> bool or func
				cout << "SEMANTICERROR: Invalid use of cin with ";
				print(pointer->type);
				cout <<endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
			//ASSEMBLER: stream extract into variable
			print_instruction("inw", pointer, "", NULL, "");
			//cout << "successfully looked up <" << pointer->node_lexeme << ">" << endl;
			the_scanner.get_next_token(the_token);
			return;
		}
		else { //ERROR expected IDENTIFIER_T
			parse_error(the_token, "IDENTIFIER");
		}
	}
	//<follow_cin> -> <e>
	else if (the_token.get_type() == END_T ||
			 the_token.get_type() == SEMICOLON_T ||
			 the_token.get_type() == ELSE_T) { //follow set
		    //ASSEMBLER: OUTPUT "CIN.GET" EQUIVALENT
			outfile << "inb @SP" << endl;
			return;
	}
	else { //ERROR expected follow_cin
		parse_error(the_token, ">> END ; ELSE");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=follow_cout
void parser_class::follow_cout()
//Precondition:  None.
//Postcondition: Parses follow_cout, returning quietly if valid and displaying an error if not
{
	//<follow_cout> -> << <cout_output> <cout_tail>
	if (the_token.get_type() == INSERT_T) {
		the_scanner.get_next_token(the_token);
		if (the_token.get_type() == OPENING_T ||
			the_token.get_type() == IDENTIFIER_T ||
			the_token.get_type() == DIGIT_T ||
			the_token.get_type() == TRUE_T ||
			the_token.get_type() == FALSE_T ||
			the_token.get_type() == STRING_T ||
			the_token.get_type() == ENDL_T) { //guard cout_output
			cout_output();
			if (the_token.get_type() == INSERT_T ||
				the_token.get_type() == ELSE_T ||
				the_token.get_type() == SEMICOLON_T ||
				the_token.get_type() == END_T) { //guard cout_tail
				cout_tail();
				return;
			}
			else { //ERROR expected cout_tail
				parse_error(the_token, "<< ELSE ; END");
			}
		}
		else { //ERROR expected cout_output
			parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE STRING ENDL");
		}
	}	 
	else { //ERROR expected follow_cout
		parse_error(the_token, "<<");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=cout_tail
void parser_class::cout_tail()
//Precondition:  None.
//Postcondition: Parses cout_tail, returning quietly if valid and displaying an error if not
{
	//<cout_tail> -> <follow_cout>
	if (the_token.get_type() == INSERT_T) { //guard follow_cout
		follow_cout();
		return;
	}
	//<cout_tail> -> <e>
	else if (the_token.get_type() == ELSE_T ||
			 the_token.get_type() == SEMICOLON_T ||
			 the_token.get_type() == END_T) { //follow set
			 return;
	}
	else { //ERROR expected cout_tail
		parse_error(the_token, "<< ELSE ; END");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=cout_output
void parser_class::cout_output()
//Precondition:  None.
//Postcondition: Parses cout_output, returning quietly if valid and displaying an error if not
{
	//<cout_output> -> <expression>
	if (the_token.get_type() == OPENING_T ||
		the_token.get_type() == IDENTIFIER_T ||
		the_token.get_type() == DIGIT_T ||
		the_token.get_type() == TRUE_T ||
		the_token.get_type() == FALSE_T) { //guard expression
		parse_expression();
		//ASSEMBLER: stream insert expression
		print_instruction("outw",NULL,"-4@SP",NULL,"");
		print_instruction("subw",NULL,"#4",NULL,"SP");
		return;
	}
	//<cout_output> -> string-constant
	else if (the_token.get_type() == STRING_T) {
			 //ASSEMBLER: PRINT STRING CONSTANT
			 for (int i = 0; i < strlen(the_token.the_lexeme); i++) {
				 if (isspace(the_token.the_lexeme[i])) {
					 outfile << "outb #" << 32 << endl;
				 }
				 else {
					 outfile << "outb ^" << the_token.the_lexeme[i] << endl;
				 }
			 }
			 the_scanner.get_next_token(the_token);
			 return;
	}
	//<cout_output> -> endl
	else if (the_token.get_type() == ENDL_T) {
			 //ASSEMBLER: PRINT ENDLINE
			 outfile << "outb #10" << endl;
			 the_scanner.get_next_token(the_token);
			 return;
	}
	else { //ERROR expected cout_output
		parse_error(the_token, "( IDENTIFIER INTEGER-CONSTANT TRUE FALSE STRING ENDL");
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=parse_expression
void parser_class::parse_expression()
//Precondition:  None.
//Postcondition: Parses expression according to YASL grammar,
//				 returns if expression is valid, or identifies
//				 syntax errors
{
	//algorithm tokens
	token_class top_terminal;
	
	//reduce markers
	token_class popped;
	token_class last_term_popped;
	bool terminal_popped = false;

	//reduce array
	token_class popped_array[5];
	int array_index = 0;

	//Pointer for Saving and value for int-const, true, false
	table_node* save_symbol_ptr = NULL;
	char tmp[5];

	//CHECK THAT IT IS NOT AN EMPTY EXPRESSION
	if (the_token.get_type() != OPENING_T &&
		the_token.get_type() != IDENTIFIER_T &&
		the_token.get_type() != DIGIT_T &&
		the_token.get_type() != TRUE_T &&
		the_token.get_type() != FALSE_T) { //ERROR empty expression
			cout << "SYNTAXERROR: Empty Expression" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
	}
	//ASSEMBLER: Save SP (top of stack) in R1
	print_instruction("movw",NULL,"SP",NULL,"R1");

	//push ; onto stack
	token_class init;
	init.update(SEMICOLON_T, NONE_ST, ";\0");
	the_stack.push(init);
	//the_scanner.get_next_token(the_token); //TOKEN ALREADY LOADED
	//the_token.display(); //debugging purposes
	
	while (1)
	{
		if (the_stack.peek_top_terminal() == SEMICOLON_T && 
			(the_token.get_type() == SEMICOLON_T || 
			 the_token.get_type() == DO_T ||
			 the_token.get_type() == THEN_T ||
			 the_token.get_type() == CLOSING_T ||
			 the_token.get_type() == ELSE_T ||
			 the_token.get_type() == COMMA_T ||
			 the_token.get_type() == INSERT_T ||
			 the_token.get_type() == END_T)) {
			    popped = the_stack.pop();
				if ((isWhile || isIf) && popped.symbol_ptr->type != BOOLEAN_TYPE) { //ERROR nonboolean while/if expression
					cout << "SEMANTICERROR: Non-boolean expression in ";
					if (isWhile) cout << "while ";
					else if (isIf) cout << "if ";
					cout << "statement" << endl;
					the_scanner.print_current_line();
					cin.get();
					exit(1);
				}
				if (isFuncCall && paramType != popped.symbol_ptr->type) { //ERROR type mismatch parameter
					cout << "SEMANTICERROR: Invalid parameter, expected <";
					print(paramType);
					cout << "> but found <";
					print(popped.symbol_ptr->type);
					cout << ">" << endl;
					the_scanner.print_current_line();
					cin.get();
					exit(1);
				}
				//ASSEMBLER: store final value in R1
				print_instruction("movw",popped.symbol_ptr,"",NULL,"@R1");
				print_instruction("movw",NULL,"R1",NULL,"SP");
				print_instruction("addw",NULL,"#4",NULL,"SP");
				expr_type = popped.symbol_ptr->type;
				expr_kind = popped.symbol_ptr->kind;
				the_scanner.s_delete_temps();
				return; //successfully parsed expression
		}
		else {
			top_terminal = the_stack.get_top_terminal();
			if (precedence(top_terminal, the_token) == LT || 
				precedence(top_terminal, the_token) == EQ) { //SHIFT
				if (the_token.get_type() == IDENTIFIER_T) {
					//CHECK FOR UNDECLARED IDENTIFIER
					pointer = the_scanner.s_table_lookup(the_token.the_lexeme);
					if (pointer == NULL) { //ERROR undeclared identifier
						undeclared_id(the_token);
					}
					the_token.symbol_ptr = pointer;
					if (pointer->type == FUNC_ID_TYPE) { //ERROR can't use functions in expressions
						cout << "SEMANTICERROR: Invalid use of function id " << pointer->node_lexeme << " in expression";
						the_scanner.print_current_line();
						cin.get();
						exit(1);
					}
					//cout << "successfully looked up <" << pointer->node_lexeme << ">" << endl;
				}
				else if (the_token.get_type() == DIGIT_T) {
					get_next_temp(tmp);
					the_scanner.s_table_add_entry(tmp,TEMP_KIND,INT_TYPE);
					the_token.symbol_ptr = the_scanner.s_get_temp_ptr();
					print_instruction("addw",NULL,"#4",NULL,"SP");
					char int_const[8] = "#";
					strcat(int_const,the_token.the_lexeme);
					strcat(int_const,"\0");
					print_instruction("movw",NULL,int_const,the_token.symbol_ptr,"");
				}
				else if (the_token.get_type() == TRUE_T) {
					get_next_temp(tmp);
					the_scanner.s_table_add_entry(tmp,TEMP_KIND,BOOLEAN_TYPE);
					the_token.symbol_ptr = the_scanner.s_get_temp_ptr();
					print_instruction("addw",NULL,"#4",NULL,"SP");
					print_instruction("movw",NULL,"#1",the_token.symbol_ptr,"");
				}
				else if (the_token.get_type() == FALSE_T) {
					get_next_temp(tmp);
					the_scanner.s_table_add_entry(tmp,TEMP_KIND,BOOLEAN_TYPE);
					the_token.symbol_ptr = the_scanner.s_get_temp_ptr();
					print_instruction("addw",NULL,"#4",NULL,"SP");
					print_instruction("movw",NULL,"#0",the_token.symbol_ptr,"");
				}
				the_stack.push(the_token);
				the_scanner.get_next_token(the_token);
				tmp[0] = '\0';
				//the_token.display(); //debugging purposes
			}
			else if (precedence(top_terminal, the_token) == GT) { //REDUCE
				do {
					popped = the_stack.pop();
					popped_array[array_index++] = popped; //save to array
				
					if (popped.get_type() != E_T) { //popped a terminal
						terminal_popped = true;
						last_term_popped.update(popped.get_type(),popped.get_subtype(),"\0");
					}
				} while ((!terminal_popped) || 
						 (the_stack.peek() == E_T) || 
						 (precedence(the_stack.get_top_terminal(), last_term_popped) != LT));
				
				if (valid_expression(popped_array, array_index, save_symbol_ptr)) {
					if (array_index == 3 && popped_array[1].get_type() != E_T) {
						evaluate_expression(popped_array, save_symbol_ptr);
					}
					//push E token onto stack
					token_class e_token;
					e_token.update(E_T, NONE_ST, "\0");
					e_token.symbol_ptr = save_symbol_ptr;
					the_stack.push(e_token);
					
					//check expression debugging
					if (the_scanner.is_expression_debugging()) print_production(popped_array, array_index);
					
					//reset array and reduce markers
					last_term_popped.update(NONE_T,NONE_ST,"\0");
					terminal_popped = false;
					array_index = 0;
					save_symbol_ptr = NULL;
				}
				else { //not valid r.h.s. expression
					cout << "SYNTAXERROR: Invalid expression" << endl;
					the_scanner.print_current_line();
					cin.get();
					exit(1);
				}
			}
			else { //precedence must have returned ER
				/*if (the_token.get_type() == EOF_T) {
					cout << "Reached End of File" << endl;
					cin.get();
					exit(1);
				}*/
				cout << "SYNTAXERROR: Invalid expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
			}
		}
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=valid_expression
bool parser_class::valid_expression(token_class expression[], int index, table_node* &save)
//Precondition:  None.
//Postcondition: Returns whether the expression popped is valid
//				 based on the context free grammar for YASL expressions
{
	if (index != 1 && index != 3) return false; //assert: will always be length 1 or 3
	switch (index) {
		case 1: if (expression[0].get_type() == IDENTIFIER_T ||
					expression[0].get_type() == DIGIT_T ||
					expression[0].get_type() == TRUE_T ||
					expression[0].get_type() == FALSE_T) {
					save = expression[0].symbol_ptr;
					return true;
				}
				else return false;
		case 3: if (expression[2].get_type() == E_T) {
					if (expression[1].get_subtype() == PLUS_ST ||
						expression[1].get_subtype() == MULTIPLY_ST ||
						expression[1].get_subtype() == MINUS_ST ||
						expression[1].get_subtype() == DIV_ST ||
						expression[1].get_subtype() == MOD_ST ||
						expression[1].get_subtype() == OR_ST ||
						expression[1].get_subtype() == AND_ST ||
						expression[1].get_subtype() == EQUAL_ST ||
						expression[1].get_subtype() == LESS_ST ||
						expression[1].get_subtype() == LESSEQ_ST ||
						expression[1].get_subtype() == GREAT_ST ||
						expression[1].get_subtype() == GREATEQ_ST ||
						expression[1].get_subtype() == NOTEQUAL_ST)
						if (expression[0].get_type() == E_T) return true;
				}
				else if (expression[2].get_type() == OPENING_T && 
						 expression[1].get_type() == E_T &&
						 expression[0].get_type() == CLOSING_T) {
						save = expression[1].symbol_ptr;
						return true;
				}
				else return false;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=evaluate_expression
void parser_class::evaluate_expression(token_class to_eval[], table_node* &to_save)
//Precondition:  None.
//Postcondition: Outputs assembly language for reductions.
{
	char temp[8];
	char skip[8];
	if (to_eval[1].get_subtype() == PLUS_ST) {
		if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE ||
			to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) { //ERROR can't add booleans
				cout << "SEMANTICERROR: Invalid use of boolean in addition expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == INT_TYPE &&
				 to_eval[0].symbol_ptr->type == INT_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,INT_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: add variables
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("addw",to_eval[0].symbol_ptr,"",to_save,"");
		}
	}
	else if (to_eval[1].get_subtype() == MINUS_ST) {
		if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE ||
			to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) { //ERROR can't subtract booleans
				cout << "SEMANTICERROR: Invalid use of boolean in subtraction expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == INT_TYPE &&
				 to_eval[0].symbol_ptr->type == INT_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,INT_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: subtract variables
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("subw",to_eval[0].symbol_ptr,"",to_save,"");
		}
	}
	else if (to_eval[1].get_subtype() == MULTIPLY_ST) {
		if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE ||
			to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) { //ERROR can't multiply booleans
				cout << "SEMANTICERROR: Invalid use of boolean in multiplication expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == INT_TYPE &&
				 to_eval[0].symbol_ptr->type == INT_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,INT_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: multiply variables
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("mulw",to_eval[0].symbol_ptr,"",to_save,"");
		}
	}
	else if (to_eval[1].get_subtype() == DIV_ST) {
		if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE ||
			to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) { //ERROR can't divide booleans
				cout << "SEMANTICERROR: Invalid use of boolean in division expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == INT_TYPE &&
				 to_eval[0].symbol_ptr->type == INT_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,INT_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: divide variables
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("divw",to_eval[0].symbol_ptr,"",to_save,"");
		}
	}
	else if (to_eval[1].get_subtype() == AND_ST) {
		if (to_eval[2].symbol_ptr->type == INT_TYPE ||
			to_eval[0].symbol_ptr->type == INT_TYPE) { //ERROR can't AND ints
				cout << "SEMANTICERROR: Invalid use of int in AND expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE &&
				 to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: AND variables
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("mulw",to_eval[0].symbol_ptr,"",to_save,"");
		}
	}
	else if (to_eval[1].get_subtype() == OR_ST) {
		if (to_eval[2].symbol_ptr->type == INT_TYPE ||
			to_eval[0].symbol_ptr->type == INT_TYPE) { //ERROR can't OR ints
				cout << "SEMANTICERROR: Invalid use of int in OR expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE &&
				 to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: OR variables
					 get_next_skip(skip);
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("addw",to_eval[0].symbol_ptr,"",to_save,"");
					 print_instruction("cmpw",to_save,"",NULL,"#0");
					 print_instruction("beq",NULL,skip,NULL,"");
					 print_instruction("movw",NULL,"#1",to_save,"");
					 outfile << skip << " movw R0 R0" << endl;
		}
	}
	else if (to_eval[1].get_subtype() == MOD_ST) {
		if (to_eval[2].symbol_ptr->type == BOOLEAN_TYPE ||
			to_eval[0].symbol_ptr->type == BOOLEAN_TYPE) { //ERROR can't mod booleans
				cout << "SEMANTICERROR: Invalid use of boolean in MOD expression" << endl;
				the_scanner.print_current_line();
				cin.get();
				exit(1);
		}
		else if (to_eval[2].symbol_ptr->type == INT_TYPE &&
				 to_eval[0].symbol_ptr->type == INT_TYPE) {
					 get_next_temp(temp);
					 the_scanner.s_table_add_entry(temp,TEMP_KIND,INT_TYPE);
					 to_save = the_scanner.s_get_temp_ptr();
					 //ASSEMBLER: mod variables
					 print_instruction("addw",NULL,"#4",NULL,"SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("divw",to_eval[0].symbol_ptr,"",to_save,"");
					 print_instruction("mulw",to_eval[0].symbol_ptr,"",to_save,"");
					 print_instruction("movw",to_save,"",NULL,"@SP");
					 print_instruction("movw",to_eval[2].symbol_ptr,"",to_save,"");
					 print_instruction("subw",NULL,"@SP",to_save,"");
		}
	}
	else if (to_eval[1].get_subtype() == EQUAL_ST) {
		if (to_eval[2].symbol_ptr->type != to_eval[0].symbol_ptr->type) { //ERROR type mismatch
			cout << "SEMANTICERROR: Type mismatch between [" << to_eval[2].the_lexeme << "] (";
			print(to_eval[2].symbol_ptr->type);
			cout << ") and [" << to_eval[0].the_lexeme << "] (";
			print(to_eval[0].symbol_ptr->type);
			cout << ")" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
		}
		get_next_temp(temp);
		the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
		to_save = the_scanner.s_get_temp_ptr();
		//ASSEMBLER: write equal routine
		get_next_skip(skip);
		print_instruction("addw",NULL,"#4",NULL,"SP");
		print_instruction("movw",NULL,"#1",to_save,"");
		print_instruction("cmpw",to_eval[2].symbol_ptr,"",to_eval[0].symbol_ptr,"");
		print_instruction("beq",NULL,skip,NULL,"");
		print_instruction("movw",NULL,"#0",to_save,"");
		outfile << skip << " movw R0 R0" << endl;
	}
	else if (to_eval[1].get_subtype() == NOTEQUAL_ST) {
		if (to_eval[2].symbol_ptr->type != to_eval[0].symbol_ptr->type) { //ERROR type mismatch
			cout << "SEMANTICERROR: Type mismatch between [" << to_eval[2].the_lexeme << "] (";
			print(to_eval[2].symbol_ptr->type);
			cout << ") and [" << to_eval[0].the_lexeme << "] (";
			print(to_eval[0].symbol_ptr->type);
			cout << ")" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
		}
		get_next_temp(temp);
		the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
		to_save = the_scanner.s_get_temp_ptr();
		//ASSEMBLER: write not equal routine
		get_next_skip(skip);
		print_instruction("addw",NULL,"#4",NULL,"SP");
		print_instruction("movw",NULL,"#1",to_save,"");
		print_instruction("cmpw",to_eval[2].symbol_ptr,"",to_eval[0].symbol_ptr,"");
		print_instruction("bneq",NULL,skip,NULL,"");
		print_instruction("movw",NULL,"#0",to_save,"");
		outfile << skip << " movw R0 R0" << endl;
	}
	else if (to_eval[1].get_subtype() == LESS_ST) {
		if (to_eval[2].symbol_ptr->type != to_eval[0].symbol_ptr->type) { //ERROR type mismatch
			cout << "SEMANTICERROR: Type mismatch between [" << to_eval[2].the_lexeme << "] (";
			print(to_eval[2].symbol_ptr->type);
			cout << ") and [" << to_eval[0].the_lexeme << "] (";
			print(to_eval[0].symbol_ptr->type);
			cout << ")" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
		}
		get_next_temp(temp);
		the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
		to_save = the_scanner.s_get_temp_ptr();
		//ASSEMBLER: write less routine
		get_next_skip(skip);
		print_instruction("addw",NULL,"#4",NULL,"SP");
		print_instruction("movw",NULL,"#1",to_save,"");
		print_instruction("cmpw",to_eval[2].symbol_ptr,"",to_eval[0].symbol_ptr,"");
		print_instruction("blss",NULL,skip,NULL,"");
		print_instruction("movw",NULL,"#0",to_save,"");
		outfile << skip << " movw R0 R0" << endl;
	}
	else if (to_eval[1].get_subtype() == LESSEQ_ST) {
		if (to_eval[2].symbol_ptr->type != to_eval[0].symbol_ptr->type) { //ERROR type mismatch
			cout << "SEMANTICERROR: Type mismatch between [" << to_eval[2].the_lexeme << "] (";
			print(to_eval[2].symbol_ptr->type);
			cout << ") and [" << to_eval[0].the_lexeme << "] (";
			print(to_eval[0].symbol_ptr->type);
			cout << ")" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
		}
		get_next_temp(temp);
		the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
		to_save = the_scanner.s_get_temp_ptr();
		//ASSEMBLER: write lesseq routine
		get_next_skip(skip);
		print_instruction("addw",NULL,"#4",NULL,"SP");
		print_instruction("movw",NULL,"#1",to_save,"");
		print_instruction("cmpw",to_eval[2].symbol_ptr,"",to_eval[0].symbol_ptr,"");
		print_instruction("bleq",NULL,skip,NULL,"");
		print_instruction("movw",NULL,"#0",to_save,"");
		outfile << skip << " movw R0 R0" << endl;
	}
	else if (to_eval[1].get_subtype() == GREAT_ST) {
		if (to_eval[2].symbol_ptr->type != to_eval[0].symbol_ptr->type) { //ERROR type mismatch
			cout << "SEMANTICERROR: Type mismatch between [" << to_eval[2].the_lexeme << "] (";
			print(to_eval[2].symbol_ptr->type);
			cout << ") and [" << to_eval[0].the_lexeme << "] (";
			print(to_eval[0].symbol_ptr->type);
			cout << ")" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
		}
		get_next_temp(temp);
		the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
		to_save = the_scanner.s_get_temp_ptr();
		//ASSEMBLER: write greater routine
		get_next_skip(skip);
		print_instruction("addw",NULL,"#4",NULL,"SP");
		print_instruction("movw",NULL,"#1",to_save,"");
		print_instruction("cmpw",to_eval[2].symbol_ptr,"",to_eval[0].symbol_ptr,"");
		print_instruction("bgtr",NULL,skip,NULL,"");
		print_instruction("movw",NULL,"#0",to_save,"");
		outfile << skip << " movw R0 R0" << endl;
	}
	else if (to_eval[1].get_subtype() == GREATEQ_ST) {
		if (to_eval[2].symbol_ptr->type != to_eval[0].symbol_ptr->type) { //ERROR type mismatch
			cout << "SEMANTICERROR: Type mismatch between [" << to_eval[2].the_lexeme << "] (";
			print(to_eval[2].symbol_ptr->type);
			cout << ") and [" << to_eval[0].the_lexeme << "] (";
			print(to_eval[0].symbol_ptr->type);
			cout << ")" << endl;
			the_scanner.print_current_line();
			cin.get();
			exit(1);
		}
		get_next_temp(temp);
		the_scanner.s_table_add_entry(temp,TEMP_KIND,BOOLEAN_TYPE);
		to_save = the_scanner.s_get_temp_ptr();
		//ASSEMBLER: write greateq routine
		get_next_skip(skip);
		print_instruction("addw",NULL,"#4",NULL,"SP");
		print_instruction("movw",NULL,"#1",to_save,"");
		print_instruction("cmpw",to_eval[2].symbol_ptr,"",to_eval[0].symbol_ptr,"");
		print_instruction("bgeq",NULL,skip,NULL,"");
		print_instruction("movw",NULL,"#0",to_save,"");
		outfile << skip << " movw R0 R0" << endl;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print_production
void parser_class::print_production(token_class to_print[], int index)
//Precondition:  None.
//Postcondition: Production used is printed to screen
{
	cout << "E -> ";
	for (int i = index-1; i >= 0; i--) {
		if (to_print[i].get_type() == E_T) cout << "E ";
		else {
			if (to_print[i].get_type() == RELOP_T) {
				if (to_print[i].get_subtype() == EQUAL_ST) cout << "== ";
				else if (to_print[i].get_subtype() == NOTEQUAL_ST) cout << "<> ";
				else if (to_print[i].get_subtype() == LESS_ST) cout << "< ";
				else if (to_print[i].get_subtype() == LESSEQ_ST) cout << "<= ";
				else if (to_print[i].get_subtype() == GREAT_ST) cout << "> ";
				else if (to_print[i].get_subtype() == GREATEQ_ST) cout << ">= ";
			}
			else if (to_print[i].get_type() == MULOP_T) {
				if (to_print[i].get_subtype() == MULTIPLY_ST) cout << "* ";
				else if (to_print[i].get_subtype() == DIV_ST) cout << "div ";
				else if (to_print[i].get_subtype() == MOD_ST) cout << "mod ";
				else if (to_print[i].get_subtype() == AND_ST) cout << "and ";
			}
			else if (to_print[i].get_type() == ADDOP_T) {
				if (to_print[i].get_subtype() == PLUS_ST) cout << "+ ";
				else if (to_print[i].get_subtype() == MINUS_ST) cout << "- ";
				else if (to_print[i].get_subtype() == OR_ST) cout << "or ";
			}
			else if (to_print[i].get_type() == OPENING_T) cout << "( ";
			else if (to_print[i].get_type() == CLOSING_T) cout << ") ";
			else if (to_print[i].get_type() == IDENTIFIER_T) cout << "id ";
			else if (to_print[i].get_type() == DIGIT_T) cout << "integer-constant ";
			else if (to_print[i].get_type() == TRUE_T) cout << "true ";
			else if (to_print[i].get_type() == FALSE_T) cout << "false ";
		}
	}
	cout << endl;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print_lines_compiled
void parser_class::print_lines_compiled()
//Precondition:  None.
//Postcondition: Calls scanner's s_print_lines_compiled
{
	the_scanner.s_print_lines_compiled();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print_instruction
void parser_class::print_instruction(char instr[], table_node* ptr1, char str1[], table_node* ptr2, char str2[])
//Precondition:  None.
//Postcondition: Prints proper assembly language instruction to out.pal
{
	if (ptr1 != NULL) outfile << "movw +" << ptr1->node_nesting_level*4 << "$display R2" << endl;
	if (ptr2 != NULL) outfile << "movw +" << ptr2->node_nesting_level*4 << "$display R3" << endl;
	int offset = 0;
	outfile << instr << " ";
	if (ptr1 == NULL) outfile << str1 << " ";
	else {
		if (ptr1->kind == VALUE_PARAM) { //value param, reference backwards from FP
			offset = 20;
			offset += (ptr1->numparams-ptr1->node_offset-1)*4;
			outfile << "-" << offset;
		}
		else if (ptr1->kind == REF_PARAM) { //ref param, double indirect reference backward from FP
			offset = 20;
			offset += (ptr1->numparams-ptr1->node_offset-1)*4;
			outfile << "-" << offset << "@";
		}
		else { //regular variable, reference from either R0 or FP
			offset = ptr1->node_offset;
			outfile << "+" << offset*4;
		}
		outfile << "@R2 ";
		/*
		//if global, reference from R0, else reference from FP
		if (ptr1->node_nesting_level == 0) outfile << "@R0 ";
		else if (ptr1->node_nesting_level > 0) outfile << "@FP ";
		*/
	}
	if (ptr2 == NULL) outfile << str2 << endl;
	else { 
		if (ptr2->kind == VALUE_PARAM) { //value param, reference backwards from FP
			offset = 20;
			offset += (ptr2->numparams-ptr2->node_offset-1)*4;
			outfile << "-" << offset;
		}
		else if (ptr2->kind == REF_PARAM) { //ref param, double indirect reference backward from FP
			offset = 20;
			offset += (ptr2->numparams-ptr2->node_offset-1)*4;
			outfile << "-" << offset << "@";
		}
		else { //regular variable, reference from either R0 or FP
			offset = ptr2->node_offset;
			outfile << "+" << offset*4;
		}
		outfile << "@R3" << endl;
		/*
		//if global, reference from R0, else reference from FP
		if (ptr2->node_nesting_level == 0) outfile << "@R0" << endl;
		else if (ptr2->node_nesting_level > 0) outfile << "@FP" << endl;
		*/
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_temp
void parser_class::get_next_temp(char temp_name[])
//Precondition:  None.
//Postcondition: Creates a new unique temp name
{
	static int next_ct = 0;
	sprintf(temp_name, "$%d", next_ct);
	next_ct++;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_skip
void parser_class::get_next_skip(char skip_name[])
//Precondition:  None.
//Postcondition: Creates a new unique skip label
{
	static int next_ct = 0;
	sprintf(skip_name, "$skip%d", next_ct);
	next_ct++;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_while
void parser_class::get_next_while(char top_while[], char end_while[])
//Precondition:  None.
//Postcondition: Creates new while labels
{
	static int next_ct = 0;
	sprintf(top_while, "$top%d", next_ct);
	sprintf(end_while, "$after%d", next_ct);
	next_ct++;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_if
void parser_class::get_next_if(char false_if[], char false_else[])
//Precondition:  None.
//Postcondition: Creates new if labels
{
	static int next_ct = 0;
	sprintf(false_if, "$if%d", next_ct);
	sprintf(false_else, "$else%d", next_ct);
	next_ct++;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_begin
void parser_class::get_next_begin(char begin_name[])
//Precondition:  None.
//Postcondition: Creates a new begin label
{
	static int next_ct = 0;
	sprintf(begin_name, "$begin%d", next_ct);
	next_ct++;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_next_special_temp
void parser_class::get_next_special_temp(char spec_name[])
//Precondition:  None.
//Postcondition: Creates a new special temp label
{
	static int next_ct = 0;
	sprintf(spec_name, "$s%d", next_ct);
	next_ct++;
}
///////////////////////////////////////////////////

///////////////////////////////////////////////////
////            PRIVATE FUNCTIONS              ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=parse_error
void parser_class::parse_error(token_class &found, string expected)
//Precondition:  None.
//Postcondition: Prints specific error based on where it
//				 occurred during the parse, prints offending
//				 line (or near it) and quits.
{
	cout << "SYNTAXERROR: Found [ "; 
		found.display_lexeme();
	cout << " ] when expecting one of [ " << expected << " ]" << endl;
		the_scanner.print_current_line();
		cin.get();
		exit(1);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=undeclared_id
void parser_class::undeclared_id(token_class &found)
//Precondition:  None.
//Postcondition: Prints undeclared identifier error
{
	cout << "SYNTAXERROR: Undeclared identifier <"; 
		found.display_lexeme();
	cout << "> found" << endl;
		the_scanner.print_current_line();
		cin.get();
		exit(1);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print
void parser_class::print(int to_print)
//Precondition:  None.
//Postcondition: Prints kind or type of symbol
{
	if (to_print == FUNC_ID) cout << "FUNC_ID";
	else if (to_print == VAR_ID) cout << "VAR_ID";
	else if (to_print == REF_PARAM) cout << "REF_PARAM";
	else if (to_print == VALUE_PARAM) cout << "VALUE_PARAM";
	else if (to_print == FUNC_ID_TYPE) cout << "FUNC_ID_TYPE";
	else if (to_print == INT_TYPE) cout << "INT_TYPE";
	else if (to_print == BOOLEAN_TYPE) cout << "BOOLEAN_TYPE";
}
///////////////////////////////////////////////////

///////////////////////////////////////////////////
////          NON-MEMBER FUNCTIONS             ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=precedence
int precedence(token_class &row, token_class &column)
//Precondition:  None.
//Postcondition: Returns the precedence value of the row token
//				 compared to the column token, or an error.
										 //  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16
{										 //rel,add,mul, op, cl, id, ic,  t,  f,sem, do,thn,els,com,ins,end,oth
	static int precedence_table[17][17] = { GT, LT, LT, LT, GT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, ER, //rel
											GT, GT, LT, LT, GT, LT, LT ,LT, LT, GT, GT, GT, GT, GT, GT, GT, ER, //add
											GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, ER, //mul
											LT, LT, LT, LT, EQ, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //op
											GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, GT, GT, GT, GT, GT, GT, ER, //cl
											GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, GT, GT, GT, GT, GT, GT, ER, //id
											GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, GT, GT, GT, GT, GT, GT, ER, //ic
											GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, GT, GT, GT, GT, GT, GT, ER, //t
											GT, GT, GT, ER, GT, ER, ER, ER, ER, GT, GT, GT, GT, GT, GT, GT, ER, //f
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //sem
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //do
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //thn
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //els
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //com
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //ins
											LT, LT, LT, LT, ER, LT, LT, LT, LT, ER, ER, ER, ER, ER, ER, ER, ER, //end
											ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, ER, //oth
										  };
	//preset to other
	int r = 16;
	int c = 16;
	if (row.get_type() < 16) r = row.get_type();
	if (column.get_type() < 16) c = column.get_type();
	return precedence_table[r][c];
}
///////////////////////////////////////////////////
