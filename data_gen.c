// **************************************************************
// Function: 	data_gen
// Description:	Used to generate input data set for data mining
// **************************************************************


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[])
{
	char	rand_char	= ' ';
	char	iter_str[256];
	char	max_str[256];
	int		max_num		= 0;
	int		iter_num	= 0;
	int		char_num	= 0;

	// Enter the iteration number	
	printf("Enter the prefered iteration number: ");
	fgets(iter_str, 256, stdin);
	iter_num	= atoi(iter_str);
	
	// Enter the possible max number 
	printf("Enter the max number per iteration: ");
	fgets(max_str, 256, stdin);
	max_num	= atoi(max_str);
	
	for (int i = 0; i < iter_num; i++)
	{
		// Generate the real characters number 
		char_num	= rand() % max_num + 1;
		printf("%d\t%d\t", i, char_num);
		for (int j = 0; j < char_num; j++)
		{
			int temp;
			temp	= rand() % 26 + 97;
			rand_char	= (char) temp;
			printf("%c\t", rand_char);
		}
		printf("\n");
	}
	return 0;
}
