#include <stdio.h>
int main(void) 
{
	//File opening and writing code from https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html
	FILE* output = fopen("output.out", "w");
	// test for files not existing. 
	if (output == NULL)
	{
		printf("Error! Could not open file\n");
	}

	// write to file vs write to screen 
	fprintf(output, "this is a test %d\n", 2); // write to file

	printf("End of code\n");
	return (0);
}