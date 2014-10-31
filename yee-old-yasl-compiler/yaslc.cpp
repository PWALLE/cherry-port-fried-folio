

//Dave A. Berque, Revised August 2009
//This driver program should be used to test the first part of the YASL
//compiler.
//Paul Elliott, Revised 6 December 2009
//Added: elements to test the new methods of file_manager_class
//Modified: Rewrote to test scanner class
//Modified: Rewrote to test parser class
//Modified: Rewrote to test the preproject (recursive descent)
//Finished: The project y'all
//This version tested in Visual Studio .NET 2008


#include "stdafx.h"  // A visual studio requirement


#include "parser.h"
#include <iostream>

int main(int argc, char* argv[])
{   
	parser_class parser; //Scanner imbedded

	parser.parse_program();

	parser.print_lines_compiled();
	
	cin.get();
	return 0;
	
	/*while (1)
	{
		cout << "About to parse an expression:" << endl;
		parser.parse_expression();
		cout << "Parsed one expression" << endl;
	}*/

	/*scanner_class scanner; //Filemanager imbedded
	token_class the_token;
	do {
		scanner.get_next_token(the_token);
		the_token.display();
	} while (the_token.get_type() != EOF_T);
	scanner.close_file();
	cin.get();
	return(0);*/

	/*file_manager_class thefile;     //Define the sourceprogram object
    char ch;
	int count = 0; //used to test pushback and print functions

	thefile.pushback(); //shouldn't affect program output

    while ((ch = thefile.get_next_char()) != EOF)
    {  
		if (count % 5 == 0) { //pushback every five characters, including the repeated (pushedback) character
			thefile.pushback();
		}
		cout << ch;
		count++;
		if (count == 6) thefile.set_print_status(true); //should only print second two lines
    }
    thefile.close_source_program();
    cout << "Successfully compiled " << thefile.num_lines_processed() << " lines of code." << endl;
	cin.get();

    return (0);*/
}

