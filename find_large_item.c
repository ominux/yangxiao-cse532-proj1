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
	LIST		curr_itemset[LIST_SIZE];
	LIST		next_itemset[LIST_SIZE];
		
	tid 	= (int)((long) arg);
	start 	= tid * ITER_NUM / PROCESSOR_NUM;	
	end		= (tid + 1) * ITER_NUM / PROCESSOR_NUM;
	
	
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
	if (DEBUG_4)
	{
		printf("Thread ID: %d Local Array --> Detail:\n", tid);
		printf("Start: %d, End: %d\n", start, end);
	}
	
	// Begin to process data
	for (i = start; i < end; i++)
	{
		char_num = original_array[i][0];
		if (DEBUG_4)
		{
			printf("Char_num: %d\n", char_num);
		}
		for (j = 1; j < char_num + 1; j++)
		{
			if (DEBUG_4)
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
	
				if (DEBUG_4)
				{
					printf("Original_Array[i][k]: %c, Index_I: %d, Index_J: %d, Couter: %d, List: %d\n", original_array[i][k], index_i, index_j, local_array[index_i][index_j].counter, local_array[index_i][index_j].iter_list[i]);
				}
			}
		}
	}
	
	// Merge the local data to global data
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

	// Debud code
	if (DEBUG_3)
	{
		printf("Thread ID: %d Local Counter & List: \n", tid);
		printf("Start: %d, End: %d\n", start, end);
		print_ARRAY(local_array);
	}

	pthread_mutex_unlock(&mutexsum);
	
	// Synchronization point: wait for all peers to finish updating global_array 
	int sync = pthread_barrier_wait(&barr);
	if (sync != 0 && sync != PTHREAD_BARRIER_SERIAL_THREAD)
	{
		printf("Could not wait on barrier\n");
		exit(-1);
	}
	if (DEBUG_2)
	{
		pthread_mutex_lock(&mutexsum);
		printf("After sync\n");
        printf("Thread ID: %d Local Counter & List: \n", tid);
        printf("Start: %d, End: %d\n", start, end);
        print_ARRAY(global_array);
		pthread_mutex_unlock(&mutexsum);
	}
	// Each thread reads the assigned part of the global_array, and generates itemsize of 2
	start   = tid * ARRAY_ROW_NUM / PROCESSOR_NUM;
    end     = (tid + 1) * ARRAY_ROW_NUM / PROCESSOR_NUM;
	if (DEBUG_3)
	{
		printf("Thread ID: %d, start = %d, end = %d \n", tid, start, end);
	}
	int index = 0;
	for (i = start; i < end; i++)
	{
		for (j = 0; j < ARRAY_COL_NUM; j++)
		{
			if (global_array[i][j].counter >= THRESHOLD)
			{
				curr_itemset[index].items[0] = i;
				curr_itemset[index].items[1] = j;
				curr_itemset[index].stats.counter = global_array[i][j].counter;
				for (k = 0; k < ITER_NUM; k++)
				{
					curr_itemset[index].stats.iter_list[k] = global_array[i][j].iter_list[k];
				}
			 	index++;
			}
		}
	}
	// print out large itemset of size 2
	printf("Thread ID: %d, Large itemset of size 2: \n", tid);
	for (i = 0; i < index; i++)
	{
		printf("%c%c ",curr_itemset[i].items[0]+97, curr_itemset[i].items[1]+97);
	}
	printf("\n");
	pthread_exit((void*) 0);
}
