#include <stdio.h>
#include <stdlib.h>

//Create the node structure
//Code based on structure provided in lecture slides
typedef struct node_ts
{
	//The value the node holds (Pointer to a string)
	char* value;
	//The next node
	struct node_ts* next;
	//The previous node
	struct node_ts* previous;
} node_t;

//Function that prints all of the values in the linked list in the correct format
void print_list(node_t head)
{
	node_t curr_node = *(head.next);
	//Loop through the list until we reach the second to last element
	while (curr_node.next ->value != "TAIL")
	{
		printf("(%s,%s,%s),", curr_node.value, curr_node.previous->value, curr_node.next->value);
		curr_node = *(curr_node.next);
	}
	//Print the last element seperately to add newline instead of a comma
	printf("(%s,%s,%s)\n", curr_node.value, curr_node.previous->value, curr_node.next->value);
}

//Adds a new node to the tail of the linked list
void add_tail(node_t tail, node_t node)
{
	//Update the second to last node
	node_t prev_node = *(tail.previous);
	prev_node.next = &node;

	//Update the tail node
	tail.previous = &node;

	//Update the new node
	node.previous = &prev_node;
	node.next = &tail;
}

//Adds a new node to the head of the linked list
void add_head(node_t head, node_t node)
{
	//Update the second to front node
	node_t next_node = *(head.next);
	next_node.previous = &node;

	//Update the head node
	head.next = &node;

	//Update the new node
	node.previous = &head;
	node.next = &next_node;
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
	node_t head_node, tail_node, test_node1, test_node2;

	//Fill in the nodes values
	head_node.value = "HEAD";
	head_node.next = &test_node1;
	head_node.previous = NULL;

	test_node1.value = "test1";
	test_node1.next = &test_node2;
	test_node1.previous = &head_node;

	test_node2.value = "test2";
	test_node2.next = &tail_node;
	test_node2.previous = &test_node1;

	tail_node.value = "TAIL";
	tail_node.next = NULL;
	tail_node.previous = &test_node2;

	//Print the values in the nodes
	print_list(head_node);
	
	//Return 0
	return (0);
}