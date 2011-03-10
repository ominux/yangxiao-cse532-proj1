// **************************************************************
// Function: 	data_gen
// Description:	Used to generate input data set for data mining
// **************************************************************
#include "GlobalVar.h"


int main (int argc, char *argv[])
{
	char	rand_char	= ' ';
	//char	iter_str[256];
	//char	max_str[256];
	int		max_num		= 0;
	int		iter_num	= 0;
	int		char_num	= 0;
<<<<<<< .mine
	int		i			= 0;
	int		j			= 0;
	int		tag_list[CHAR_NUM_MAX];
=======
	int 	i,j;
>>>>>>> .r6
	FILE	*fp;
		
	// Write to a file
	fp	= fopen("trans_list.txt", "w+");	

	// Enter the iteration number	
	/*printf("Enter the prefered iteration number: ");
	fgets(iter_str, 256, stdin);
	iter_num	= atoi(iter_str);*/
	// Enter the possible max number 
	/*printf("Enter the max number per iteration: ");
	fgets(max_str, 256, stdin);
	max_num	= atoi(max_str);*/
	
	//fprintf(fp, "%d\n", iter_num);
<<<<<<< .mine
	for (i = 0; i < ITER_NUM; i++)
=======

	for (i = 0; i < iter_num; i++)
>>>>>>> .r6
	{
		// Initialize char_list
		for (j = 0; j < CHAR_NUM_MAX; j++)
		{
			tag_list[j] = 0;
		}
		// Generate the real characters number 
		char_num = rand() % CHAR_NUM_MAX + 1;
		fprintf(fp, "%d\t", char_num);
		for (j = 0; j < char_num; j++)
		{
			int temp;
			do
			{
				temp = rand() % 26;
			}
			while (tag_list[temp]);
			tag_list[temp] = 1;	
			rand_char = (char) (temp + 97);
			fprintf(fp, "%c\t", rand_char);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}
