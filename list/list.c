#include <stdio.h>
#include <stdlib.h>

//Getting arguments from command line code from https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/command_line_args.html
int main( int num_args, char* list_of_args[])
{
	//File opening and writing code from https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html
	//Open the input and output files
	FILE* input_file = fopen(list_of_args[1], "r");
	FILE* output_file = fopen(list_of_args[2], "w");

	//Test for files not existing. 
	if (!input_file || !output_file)
	{
		printf("Error! Could not open file\n");
		exit(-1);
	}

	//Read from the file and write it to the output
	char line[100];
	while (fgets(line, 100, input_file) != NULL)
	{
		printf("Line: %s", line);
	}

	//Return 0
	return (0);
}