//Dave A. Berque, Revised August 2009
//This file contains method implementations for the
//file_manager_class
//Paul Elliott, Revised 3 September 2009
//Added: Routine definitions for member functions
//		 pushback(), print_current_line(), 
//		 set_print_status(bool), num_lines_processed()
//Modified: get_next_char() to incorporate printing functions
//Modified: num_lines_processed now prints lines processed, not current line number

//The class header is found in filemngr.h

#include "stdafx.h"  // Required for visual studio

#include "filemngr.h"


///////////////////////////////////////////////////
//name=file_manager_class
file_manager_class::file_manager_class()
//Precondition: None.
//Postcondition: The constructor has opened the file which is to be
//               processed and has inialized the current line number to 0
{  char filename[MAX_CELLS_PER_STRING];

   cout << "Enter file name to compile: ";
   cin.getline (filename, MAX_CELLS_PER_STRING);
   file_to_compile.open(filename);

   if (file_to_compile.fail())
   {  cout << "Error, the file: " << filename << " was not opened." << endl;
      cout << "Press return to terminate program." << endl;
      cin.get();
	  cin.get();
      //exit(1);
   }  
   else
	     //cout << "here\n";

  current_line_number = 0;
  auto_print_status = false;
}
///////////////////////////////////////////////////



///////////////////////////////////////////////////
//name=get_next_char
int file_manager_class::get_next_char()
//Precondition:  The source file associated with the owning object has
//               been prepared for reading.
//Postcondition: The next character from the input file has been read and
//               returned by the function.  If another chacter could not
//               be read because END OF FILE was reached then EOF is
//               returned.
{  if ((current_line_number == 0) ||
      (char_to_read_next == strlen(current_line)))
   {  if (file_to_compile.peek() == EOF) return (EOF);
	   file_to_compile.getline(current_line, MAX_CELLS_PER_STRING);
      strcat(current_line, "\n");
      current_line_number++;
      char_to_read_next = 0;
	  if (auto_print_status == true) print_current_line();
   }
   return(current_line[char_to_read_next++]);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=close_source_program
void file_manager_class::close_source_program()
//Precondition:  The file belonging to the object owning this routine has
//               been opened.
//Postcondition: The file belonging to the object owning this routine has
//               been closed.
{  file_to_compile.close();
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=pushback
//Precondition:  Index char_to_read_next is not already zero
//Postcondition: char_to_read_next is decremented, returning
//				 last character to the buffer.
void file_manager_class::pushback()
{
	if (char_to_read_next > 0) char_to_read_next--;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print_current_line
//Precondition:  The file belonging to the object owning this routine
//				 is being read.
//Postcondition: Current line number and that line's contents are printed.
void file_manager_class::print_current_line()
{
	cout << current_line_number << "- " << current_line;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=set_print_status
//Precondition:  None.
//Postcondition: auto_print_status is changed to match newStatus param.
void file_manager_class::set_print_status(bool newStatus)
{
	auto_print_status = newStatus;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=num_lines_processed
//Precondition:  The file belonging to the object owning this routine
//				 is being read.
//Postcondition: The number of lines that have been processed is returned
int file_manager_class::num_lines_processed()
{
	return current_line_number;
}
///////////////////////////////////////////////////