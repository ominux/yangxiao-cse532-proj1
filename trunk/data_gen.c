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

	FILE	*fp;
	
	// Write to a file
	fp	= fopen("trans_list.txt", "w+");	

	// Enter the iteration number	
	printf("Enter the prefered iteration number: ");
	fgets(iter_str, 256, stdin);
	iter_num	= atoi(iter_str);
	
	// Enter the possible max number 
	printf("Enter the max number per iteration: ");
	fgets(max_str, 256, stdin);
	max_num	= atoi(max_str);
	
	//fprintf(fp, "%d\n", iter_num);

	for (int i = 0; i < iter_num; i++)
	{
		// Generate the real characters number 
		char_num	= rand() % max_num + 1;
		fprintf(fp, "%d\t", char_num);
		for (int j = 0; j < char_num; j++)
		{
			int temp;
			temp	= rand() % 26 + 97;
			rand_char	= (char) temp;
			fprintf(fp, "%c\t", rand_char);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}
