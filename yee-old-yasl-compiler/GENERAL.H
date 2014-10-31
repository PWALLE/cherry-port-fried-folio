//Dave A. Berque, Revised August 2009
//This file contains some general constant definitions that are likely to
//be useful in several parts of the compiler.  Feel free to add
//additional definitions here.
//Paul Elliott, Revised 6 December 2009
//Added: Definitions for token_class types and subtypes
//Modified: Switched STREAMOP and PAREN to INSERT/EXTRACT, OPENING/CLOSING,
//			renumbered everything
//Modified: renumbered so DO_T works in the parser precedence table
//Added: Definitions for symbol table constants
//Finished: Temp symbol constants

#ifndef _general
#define _general

const int MAX_CELLS_PER_STRING = 82;  //80 characters plus \n plus \0.

//Definition of token_class types
const int RELOP_T = 0;
const int ADDOP_T = 1;
const int MULOP_T = 2;
const int OPENING_T = 3;
const int CLOSING_T = 4;
const int IDENTIFIER_T = 5;
const int DIGIT_T = 6;
const int TRUE_T = 7;
const int FALSE_T = 8;
const int SEMICOLON_T = 9;
const int DO_T = 10;
const int THEN_T = 11;
const int ELSE_T = 12;
const int COMMA_T = 13;
const int INSERT_T = 14;
const int END_T = 15;
const int REFERENCE_T = 16;
const int SWAP_T = 17;
const int IF_T = 18;
const int ASSIGN_T = 19;
const int PERIOD_T = 20;
const int WHILE_T = 21;
const int STRING_T = 22;
const int PROGRAM_T = 23;
const int FUNCTION_T = 24;
const int BEGIN_T = 25;
const int EXTRACT_T = 26;
const int COUT_T = 27;
const int CIN_T = 28;
const int ENDL_T = 29;
const int INT_T = 30;
const int BOOLEAN_T = 31;
const int NONE_T = 32;
const int EOF_T = 33;

//Definition of token_class subtypes
const int EQUAL_ST = 34;
const int NOTEQUAL_ST = 35;
const int LESS_ST = 36;
const int LESSEQ_ST = 37;
const int GREAT_ST = 38;
const int GREATEQ_ST = 39;
const int PLUS_ST = 40;
const int MINUS_ST = 41;
const int OR_ST = 42;
const int AND_ST = 43;
const int DIV_ST = 44;
const int MOD_ST = 45;
const int MULTIPLY_ST = 46;
const int NONE_ST = 47;

//Definitions for Context Free Grammar E token
const int E_T = 48;

//Definitions for greater than, less than, equal, and error (for operator precedence parsing)
const int GT = 49;
const int LT = 50;
const int EQ = 51;
const int ER = 52;

//Definitions for symbol table characteristics
const int FUNC_ID = 53;
const int VAR_ID = 54;
const int REF_PARAM = 55;
const int VALUE_PARAM = 56;
const int FUNC_ID_TYPE = 57;
const int INT_TYPE = 58;
const int BOOLEAN_TYPE = 59;

const int TEMP_KIND = 60;
const int SPECIAL_TEMP_KIND = 61;
const int SPECIAL_TEMP_TYPE = 62;
const int SUPER_SPECIAL_TEMP_KIND = 63;

#endif