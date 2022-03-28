#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Create the node structure
//Code based on structure provided in lecture slides
typedef struct node_ts
{
	//The value the node holds
	char* value;
	//The next node
	struct node_ts* next;
	//The previous node
	struct node_ts* previous;
} node_t;

//Function that prints all of the values in the linked list in the correct format
void print_list(node_t* head, FILE* output_file)
{
	node_t* curr_node = head->next;
	//Check to see if the list is empty
	if (strcmp(curr_node->value, "TAIL") == 0)
	{
		fprintf(output_file,"EMPTY\n");
		return;
	}
	//Loop through the list until we reach the second to last element
	while (strcmp(curr_node->next -> value, "TAIL") != 0)
	{
		fprintf(output_file, "(%s,%s,%s),", curr_node->value, curr_node->previous->value, curr_node->next->value);
		curr_node = curr_node->next;
	}
	//Print the last element seperately to add newline instead of a comma
	fprintf(output_file, "(%s,%s,%s)\n", curr_node->value, curr_node->previous->value, curr_node->next->value);
}

//Adds a new node to the tail of the linked list
void add_tail(node_t* tail, node_t* node)
{
	//Update the second to last node
	node_t* prev_node = tail->previous;
	prev_node->next = node;

	//Update the tail node
	tail->previous = node;

	//Update the new node
	node->previous = prev_node;
	node->next = tail;
}

//Adds a new node to the head of the linked list
void add_head(node_t* head, node_t* node)
{
	//Update the second to front node
	node_t* next_node = head->next;
	next_node->previous = node;

	//Update the head node
	head->next = node;

	//Update the new node
	node->previous = head;
	node->next = next_node;
}

//Deletes all nodes with a given value
void del_node(node_t* head, char* value)
{
	node_t* curr_node = head->next;
	//Traverse the list checking for the target value
	while (strcmp(curr_node->value, "TAIL") != 0)
	{
		//Create a temporary node used when freeing up memory
		node_t* temp;
		//If the value is the targeted value, remove the node from the list
		if (strcmp(curr_node->value, value) == 0)
		{
			curr_node->previous->next = curr_node->next;
			curr_node->next->previous = curr_node->previous;
			//Free up the deleted node
			temp = curr_node->previous;
			free(curr_node->value);
			free(curr_node);
			curr_node = temp;
		}
		curr_node = curr_node->next;
	}
}

//Parse the input and make sure that it is valid, then execute proper command
void parse_input(char input[], node_t* head, node_t* tail, FILE* output_file)
{
	//Define some constants to use in strtok
	char space[2] = " ";
	char quotation[2] = "\"";

	//Find the command and argument
	char* command = strtok(input, space);
	char* argument = strtok(NULL, quotation);

	//Handle empty lines by simply returning
	if (command == NULL)
	{
		return;
	}

	//Determine the correct action to take
	if (strcmp(command, "addtail") == 0)
	{
		//Throw an error for invalid arguments
		if (strcmp(argument, "EMPTY") == 0 | strcmp(argument, "HEAD") == 0 | strcmp(command, "TAIL") == 0)
		{
			printf("Error: %s is a reserved keyword\n", argument);
			exit(-1);
		}

		//Create a new node and add it to the tail
		node_t* new_node = (node_t*)malloc(1 * sizeof(node_t));
		char* new_string = (char*)malloc(1 * sizeof(argument));
		strcpy(new_string, argument);
		new_node->value = new_string;
		add_tail(tail, new_node);
	}
	else if (strcmp(command, "addhead") == 0)
	{
		//Throw an error for invalid arguments
		if (strcmp(argument, "EMPTY") == 0 | strcmp(argument, "HEAD") == 0 | strcmp(command, "TAIL") == 0)
		{
			printf("Error: %s is a reserved keyword\n", argument);
			exit(-1);
		}

		//Create a new node and add it to the head
		node_t* new_node = (node_t*)malloc(1 * sizeof(node_t));
		char* new_string = (char*)malloc(1 * sizeof(argument));
		strcpy(new_string, argument);
		new_node->value = new_string;
		add_head(head, new_node);
	}
	else if (strcmp(command, "del") == 0)
	{
		//Throw an error for invalid arguments
		if (strcmp(argument, "EMPTY") == 0 | strcmp(argument, "HEAD") == 0 | strcmp(command, "TAIL") == 0)
		{
			printf("Error: %s is a reserved keyword\n", argument);
			exit(-1);
		}

		//Remove the target character from the list
		del_node(head, argument);
	}
	else if (strcmp(command, "print") == 0)
	{
		print_list(head, output_file);
	}
	else
	{
		printf("Error: Invalid command %s\n", command);
		exit(-1);
	}
}

int main( int num_args, char* list_of_args[])
{
	//Exit the program if there is an issue with the arguments
	if (num_args != 3)
	{
		printf("Error: expected 2 arguments but found %d\n", num_args - 1);
		exit(-1);
	}

	//Open the input and output files
	//Getting arguments from command line code from https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/command_line_args.html
	//File opening and writing code from https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html
	FILE* input_file = fopen(list_of_args[1], "r");
	FILE* output_file = fopen(list_of_args[2], "w");

	//Exit the program if there there is an issue with the files 
	if (!input_file || !output_file)
	{
		printf("Error: could not open file\n");
		exit(-1);
	}

	//Create the head node and tail nodes
	node_t head_node, tail_node;

	//Fill in the nodes values
	head_node.value = "HEAD";
	head_node.next = &tail_node;
	head_node.previous = NULL;

	tail_node.value = "TAIL";
	tail_node.next = NULL;
	tail_node.previous = &head_node;

	//Loop through the input file and execute each command
	//Code for while loop from https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html
	char line[100];
	while (fgets(line, 100, input_file) != NULL)
	{
		//Cut off the newline character at the end of the input
		//Code from https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
		line[strcspn(line, "\n")] = 0;
		//Parse the input and perform the correct action
		parse_input(line, &head_node, &tail_node, output_file);
	}
	
	//Return and automatically free up all of the memory being used
	return (0);
}