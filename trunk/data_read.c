// **************************************************************
// Function:    data_read
// Description: Used to read the raw data
// **************************************************************
#include "GlobalVar.h"        
            
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

    for (i = 0; i < ITER_NUM; i++)
    {
        fscanf(fp, "%d\t", &char_num);
        //printf("%d", char_num);
        for (j = 0; j < char_num; j++)
        {   
            fscanf(fp, "%c\t", &original_array[i][j]);
            //printf("%c", original_array[i][j]);
        }   
        //printf("\n");   
    }
    fclose(fp);
	return 1;
}

