//Paul Elliott, Revised 6 December 2009
////This file contains method implementations for the
//symbol table class
//Finished: implementations for proj6/7 functions

//The class header is found in table.h

#include "stdafx.h"
#include "table.h"
#include "string.h"

///////////////////////////////////////////////////
////           DEFINITION FOR TABLE            ////
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//name=table
table::table()
//Precondition:  None.
//Postcondition: The table is initialized to be empty
{
	current_nesting_level = 0;
	top = NULL;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=table_add_level
void table::table_add_level(char level_name[])
//Precondition:  None.
//Postcondition: Adds a new table_level to the symbol
//				 table when a new scope level is entered.
{
	table_level* temp = new table_level;
	temp->name[0] = '\0';
	if (current_nesting_level > 0) { //Concat function name onto level name
		strcpy(temp->name, top->name);
		strcat(temp->name, ".");
	}
	strcat(temp->name, level_name);
	strcat(temp->name, "\0");
	temp->nesting_level = current_nesting_level++;
	temp->next_offset = 0;
	temp->list = NULL;
	temp->next_level = top;
	top = temp;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=table_del_level
void table::table_del_level()
//Precondition:  The table is not empty
//Postcondition: Pops a table_level off the symbol table
//				 when a scope level is exited.
{
	if (top == NULL) {
		cout << "ERROR: Table is empty, cannot delete level" << endl;
		exit(1);
	}
	//pointers to assist freeing memory
	table_node* deleter;
	table_node* param_deleter;
	while (top->list != NULL) { //Free table_nodes
		deleter = top->list;
		while (top->list->parameters != NULL) { //Free function parameters
			param_deleter = top->list->parameters;
			top->list->parameters = param_deleter->next_node;
			delete param_deleter;
		}
		top->list = deleter->next_node;
		delete deleter;
	}
	//Pop table_level
	table_level* deleted_level = top;
	top = top->next_level;
	delete deleted_level;
	current_nesting_level--;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=table_add_entry
void table::table_add_entry(char sym_name[], int sym_kind, int sym_type)
//Precondition:  None.
//Postcondition: Adds an entry to the table, adding parameters to the
//				 parameter list of the correct function on the previous
//				 scope level
{
	if (sym_kind == REF_PARAM ||
		sym_kind == VALUE_PARAM) { //Add parameter to correct function in previous scope level
			table_node* temp = new table_node;
			temp->node_lexeme[0] = '\0';
			strcpy(temp->node_lexeme, sym_name);
			strcat(temp->node_lexeme, "\0");
			temp->node_nesting_level=current_nesting_level-1;
			temp->parameters=NULL;
			temp->kind = sym_kind;
			temp->type = sym_type;
			temp->numparams = 0;
			temp->next_node = NULL;
			add_param(temp);
	}
	//Add entry to current scope level
	table_node* temp = new table_node;
	temp->node_lexeme[0] = '\0';
	strcpy(temp->node_lexeme, sym_name);
	strcat(temp->node_lexeme, "\0");
	temp->kind = sym_kind;
	temp->type = sym_type;
	temp->numparams = 0;
	temp->parameters = NULL;
	temp->node_offset = top->next_offset;
	if (temp->kind != FUNC_ID) top->next_offset++; //Increment table_level's offset unless func_id
	temp->node_nesting_level = top->nesting_level;
	temp->next_node = top->list;
	top->list = temp;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=table_lookup
table_node* table::table_lookup(char compare[])
//Precondition:  None.
//Postcondition: Checks that identifier has been declared,
//				 if so returns a pointer to that symbol, otherwise
//				 returns a NULL pointer.  Puts looked up identifier
//				 in the front of the list.
{
	table_node* pointer;
	table_level* stack_pointer = top;
	table_node* prev;
	table_node* to_move;
	while (stack_pointer != NULL) { //Check current and previous scope levels
		pointer = stack_pointer->list;
		while (pointer != NULL) { //Check symbols for identifier
			if (stricmp(pointer->node_lexeme,compare) == 0) {
				if (stack_pointer->list == pointer) return pointer; //Front of list, no change
				else {
					//Position prev pointer
					prev = stack_pointer->list;
					while (prev->next_node != pointer) prev = prev->next_node;
					//Move pointer to front of list
					prev->next_node = pointer->next_node;
					pointer->next_node = stack_pointer->list;
					stack_pointer->list = pointer;
					return pointer;
				}
			}
			pointer = pointer->next_node;
		}
		stack_pointer = stack_pointer->next_level;
	}
	return NULL; //Undeclared identifier
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_temp_ptr
table_node* table::get_temp_ptr()
//Precondition:  None.
//Postcondition: Returns a pointer to the temp that was just added
{
	if (top->list->kind == TEMP_KIND) return top->list;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//namedelete_temps
void table::delete_temps()
//Precondition:  None.
//Postcondition: Removes all temps and decrements top level's offset appropriately
{
	table_node* pointer = top->list;
	table_node* deleter;
	while (pointer != NULL && pointer->next_node != NULL) {
		if (pointer->next_node->kind == TEMP_KIND) {
			deleter = pointer->next_node;
			pointer->next_node = deleter->next_node;
			delete deleter;
			top->next_offset--;
		}
		pointer = pointer->next_node;
	}
	if (top->list->kind == TEMP_KIND) {
		deleter = top->list;
		top->list = deleter->next_node;
		delete deleter;
		top->next_offset--;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//namedelete_special_temps
void table::delete_special_temps()
//Precondition:  None.
//Postcondition: Removes all special temps and decrements top level's offset appropriately
{
	table_node* pointer = top->list;
	table_node* deleter;
	while (pointer != NULL && pointer->next_node != NULL) {
		if (pointer->next_node->kind == SPECIAL_TEMP_KIND) {
			deleter = pointer->next_node;
			pointer->next_node = deleter->next_node;
			delete deleter;
			top->next_offset--;
		}
		pointer = pointer->next_node;
	}
	if (top->list->kind == SPECIAL_TEMP_KIND) {
		deleter = top->list;
		top->list = deleter->next_node;
		delete deleter;
		top->next_offset--;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print_symbol_table
void table::print_symbol_table()
//Precondition:  None.
//Postcondition: Dumps entire symbol table to output
{
	table_level* pointer = top;
	table_node* node_pointer;
	table_node* param_pointer;
	cout << "****************************************" << endl;
	while (pointer != NULL) {
		cout << "Name = " << pointer->name << " Nesting level = " << pointer->nesting_level << endl;
		node_pointer = pointer->list;
		while (node_pointer != NULL) {
			cout << "lexeme=" << node_pointer->node_lexeme << ", kind=";
			print(node_pointer->kind);
			cout << ", type=";
			print(node_pointer->type); 
			cout << ", offset=" << node_pointer->node_offset << ", nesting level " << node_pointer->node_nesting_level << endl;
			if (node_pointer->kind == FUNC_ID) {
				cout << "   param-list:" << endl;
				param_pointer = node_pointer->parameters;
				while (param_pointer != NULL) {
					cout << "      lexeme=" << param_pointer->node_lexeme << ", kind="; 
					print(param_pointer->kind);
					cout << ", type=";
					print(param_pointer->type); 
					cout << ", offset=" << param_pointer->node_offset << endl;
					param_pointer = param_pointer->next_node;
				}
			}
			node_pointer = node_pointer->next_node;
		}
		cout << endl;
		pointer = pointer->next_level;
	}
	cout << "****************************************" << endl;
	cout << endl;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=insert_func
void table::insert_func(char check[],int num)
//Precondition:  None.
//Postcondition: inserts the function label number into the offset field
{
	if (stricmp(top->list->node_lexeme, check) == 0) top->list->node_offset = num;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_param
bool table::get_param(char func_check[],int check_offset,int &kind,int &type)
//Precondition:  None.
//Postcondition: returns true if success and updates kind and type, false otherwise
{
	table_level* level_pointer = top;
	table_node* pointer;
	table_node* param_pointer;
	while (level_pointer != NULL) {
		pointer = level_pointer->list;
		while (pointer != NULL) {
			if (stricmp(pointer->node_lexeme,func_check) == 0) {
				param_pointer = pointer->parameters;
				while (param_pointer != NULL) {
					if (param_pointer->node_offset == check_offset) {
						kind = param_pointer->kind;
						type = param_pointer->type;
						return true;
					}
					param_pointer = param_pointer->next_node;
				}
				return false;
			}
			pointer = pointer->next_node;
		}
		level_pointer = level_pointer->next_level;
	}
	return false;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=reset_offset
void table::reset_offset()
//Precondition:  None.
//Postcondition: Resets offsets so locals are correct on PAL stack
{
	top->next_offset = 0;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=insert_num_params
void table::insert_num_params(int p)
//Precondition:  None.
//Postcondition: Inserts the number of parameters into the function's parameters
//				 in the symbol table, for later PAL functionality.
{
	table_node* pointer = top->list;
	int count = 0;
	while (count < p) {
		if (pointer->kind == VALUE_PARAM || pointer->kind == REF_PARAM) {
			pointer->numparams = p;
		}
		count++;
		pointer = pointer->next_node;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=get_top_nesting_level
int table::get_top_nesting_level()
//Precondition:  None.
//Postcondition: Returns the top nesting level
{
	return top->nesting_level;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=check_duplicate_id
bool table::check_duplicate_id(char check_lex[])
//Precondition:  None.
//Postcondition: Checks current scope level for a duplicate identifier
{
	table_node* list_pointer = top->list;
	//table_node* param_pointer;
	while (list_pointer != NULL) {
		if (stricmp(check_lex, list_pointer->node_lexeme) == 0) { //ERROR duplicate identifier
			return true;
		}
		/*param_pointer = list_pointer->parameters;
		while (param_pointer != NULL) {
			if (stricmp(check_lex, param_pointer->node_lexeme) == 0) { //ERROR duplicate identifier
				return true;
			}
			param_pointer = param_pointer->next_node;
		}*/ //CHECK
		list_pointer = list_pointer->next_node;
	}
	return false;
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=add_param
void table::add_param(table_node* to_add)
//Precondition:  None.
//Postcondition: Adds parameter to its function table_node in the previous scope level
{
	table_node* pointer;
	if (top->next_level->list->parameters == NULL) {
		to_add->node_offset = 0;
		top->next_level->list->parameters = to_add;
	}
	else {
		pointer = top->next_level->list->parameters;
		while (pointer->next_node != NULL) {
			pointer = pointer->next_node;
		}
		to_add->node_offset = pointer->node_offset+1;
		pointer->next_node=to_add;
	}
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//name=print
void table::print(int to_print)
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