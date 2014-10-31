//Dave A. Berque, Revised August 2009
//This file contains a class definition for managing the
//file that contains the YASL source program to be compiled.
//Paul Elliott, Revised 3 September 2009
//Added: Header definitions for member functions pushback(),
//		 print_current_line(), set_print_status(bool),
//       num_lines_processed() and private variable
//       auto_print_status

//The definitions of these routines are found in filemngr.cpp

#ifndef _filemngr
#define _filemngr

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>    //to allow exit
#include "general.h"

using namespace std;
using std::ifstream;

class file_manager_class
{ public:
     file_manager_class();
     int get_next_char();
     void close_source_program();
	 void pushback();
	 void print_current_line();
	 void set_print_status(bool newStatus);
	 int num_lines_processed();

  private:
     char current_line[MAX_CELLS_PER_STRING];
     int char_to_read_next;
     int current_line_number;
     ifstream file_to_compile;
	 bool auto_print_status;
}  ;

#endif
