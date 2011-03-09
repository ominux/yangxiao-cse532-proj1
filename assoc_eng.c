// **************************************************************
// Function: 	assoc_gen
// Description:	Used to use association rule to the dataset
// **************************************************************


#include <stdio.h>
#include <stdlib.h>

#define ITER_NUM 100
#define CHAR_NUM 100
#define PROCESSOR_NUM 10

char data_array[ITER_NUM][CHAR_NUM + 1];

int assoc_gen(int threshold)
{
	int 	char_num;
	FILE	*fp;
	

	fp	= fopen("trans_list.txt", "r+");
	
	if (fp == NULL)
	{
		perror("Can't open input file!");
	}

	//fscanf(fp, "%d", &iter_num);
	//printf("%d\n", iter_num);

	for (int i = 0; i < ITER_NUM; i++)
	{
		fscanf(fp, "%d\t", &char_num);
		printf("%d", char_num);
		for (int j = 0; j < char_num; j++)
		{
			fscanf(fp, "%c\t", &data_array[i][j]);
			printf("%c", data_array[i][j]);
		}
		printf("\n");
	}
	fclose(fp);
}
