// **************************************************************
// Function:    data_read
// Description: Used to read the raw data
// **************************************************************
#include "GlobalVar.h"        
void merge_sort(char *array_pt, int low_bound, int up_bound);       

int data_read(void)
{      
	int		i;
	int		j; 
    int     char_num;
    FILE    *fp;

    fp  = fopen("trans_list.txt", "r+");
    
    if (fp == NULL)
    {
        printf("Can't open input file!");
		return 0;
    }
	
	// Scan in the raw data
    for (i = 0; i < ITER_NUM; i++)
    {
        fscanf(fp, "%d\t", &char_num);
		original_array[i][0] = char_num;
        for (j = 1; j < char_num + 1; j++)
        {   
            fscanf(fp, "%c\t", &original_array[i][j]);
        }   
    }
    fclose(fp);

	// Merge sort
	/*
	for (i = 0; i < ITER_NUM; i++)
	{
		char_num = original_array[i][0];
		merge_sort(&original_array[i][0], 1, char_num);
	}
	*/

	// Debug code
	if (DEBUG_1)
	{
		printf("READ IN ARRAY:\n");
		for (i = 0; i < ITER_NUM; i++)
		{
			char_num = original_array[i][0];
			printf("%d\n", i);
			printf("%d\t", char_num);
			for (j = 1; j < char_num + 1; j++)
			{   
				printf("%c\t", original_array[i][j]);
			}   
			printf("\n");   
		}
	}
	return 1;
}

