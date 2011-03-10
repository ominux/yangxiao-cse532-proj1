// ***************************************************************************
// Function: 	find_large_item	
// Description:	Used to run the code
// NOTE:
//		char_num:	Stands for the actuall items in one particular iteration
// ***************************************************************************
#include "GlobalVar.h"

void *find_large_item(void *arg)
{
	int 	i;
	int		j;
	int		k;
	int		start;
	int		end;
	int 	tid;
	int		char_num;

	CELLDATA 	local_array[ARRAY_ROW_NUM][ARRAY_COL_NUM];
		
	tid 	= (int)((long) arg);
	start 	= tid * ITER_NUM / PROCESSOR_NUM;	
	end		= (tid + 1) * ITER_NUM / PROCESSOR_NUM;
	
	//printf("Thread ID: %d Initialize Local Array!\n", tid);
	
	// Initialzie local_array, clear counter and iteration list
	for (i = 0; i < ARRAY_ROW_NUM; i++)
	{
		for (j = 0; j < ARRAY_COL_NUM; j++)
		{
			local_array[i][j].counter = 0;
			for (k = 0; k < ITER_NUM; k++)
			{
				local_array[i][j].iter_list[k] = 0;
			}
		}
	}	
	
	// Scan the raw data from the start iter to end iter
	if (DEBUG_3)
	{
		printf("Thread ID: %d Local Array --> Detail:\n", tid);
		printf("Start: %d, End: %d\n", start, end);
	}	
	for (i = start; i < end; i++)
	{
		// Get the real item number of the iter being scanned
		char_num = original_array[i][0];
		if (DEBUG_3)
		{
			printf("Char_num: %d\n", char_num);
		}
		for (j = 1; j < char_num + 1; j++)
		{
			if (DEBUG_3)
			{
				printf("I: %d, J: %d, Original_Array[i][j]: %c\n", i, j, original_array[i][j]);
			}
			for (k = j + 1; k < char_num + 1; k++)
			{
				int index_i = 0;
				int index_j = 0;
				if (original_array[i][j] < original_array[i][k])
				{
					index_i = original_array[i][j] - 97;
					index_j = original_array[i][k] - 97;
					local_array[index_i][index_j].counter++;
					local_array[index_i][index_j].iter_list[i] = 1;
				}
				else if (original_array[i][j] > original_array[i][k])
				{
					index_i = original_array[i][k] - 97;
					index_j = original_array[i][j] - 97;
					local_array[index_i][index_j].counter++;
					local_array[index_i][index_j].iter_list[i] = 1;
				}
	
				if (DEBUG_3)
				{
					printf("Original_Array[i][k]: %c, Index_I: %d, Index_J: %d, Couter: %d, List: %d\n", original_array[i][k], index_i, index_j, local_array[index_i][index_j].counter, local_array[index_i][index_j].iter_list[i]);
				}
			}
		}
	}
	
	// Merge the local data to global data
	//printf("Thread ID: %d Merge Begin!\n", tid);
	pthread_mutex_lock(&mutexsum);

	for (i = 0; i < ARRAY_ROW_NUM; i++)
	{
		for (j = 0; j < ARRAY_COL_NUM; j++)
		{
			global_array[i][j].counter = global_array[i][j].counter + local_array[i][j].counter;
			for (k = start; k < end; k++)
			{
				global_array[i][j].iter_list[k] = local_array[i][j].iter_list[k];
			}
		}
	}


	if (DEBUG_1)
	{
		printf("Thread ID: %d Local Array --> Counter:\n", tid);
		printf("Start: %d, End: %d\n", start, end);
		for (i = 0; i < ARRAY_ROW_NUM; i++)
		{
			for (j = 0; j < ARRAY_COL_NUM; j++)
			{
				printf("%d\t", local_array[i][j].counter);
			}
			printf("\n");
		}
		printf("\n");
	}


	if (DEBUG_2)
	{
		for (i = 0; i < ARRAY_ROW_NUM; i++)
		{
			for (j = 0; j < ARRAY_COL_NUM; j++)
			{
				printf("Thread ID: %d Local Array [%d, %d] --> Iter_list:\n", tid, i, j);
				for (k = start; k < end; k++)
				{
					printf("%d\t", local_array[i][j].iter_list[k]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
	
	pthread_mutex_unlock(&mutexsum);
	pthread_exit((void*) 0);
}
