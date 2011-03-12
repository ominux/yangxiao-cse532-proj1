// **************************************************************
// Function: 	main	
// Description:	Used to run the code
// **************************************************************
#include "GlobalVar.h"


// Function declearation
int data_read(void);
void *find_large_item(void *arg);





int main(void)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int threshold = 0;
	void *status;

	// Read the data from TXT file
	if (data_read() == 0)
	{
		return 0;
	}

	if (DEBUG_1 || DEBUG_2 || DEBUG_3)
	{	
		printf("ITER_NUM: %d, PROCESSOR_NUM: %d\n", ITER_NUM, PROCESSOR_NUM);	
	}

    	pthread_attr_t attr;
	pthread_t thread[PROCESSOR_NUM];
		
	// Barrier initialization
	if (pthread_barrier_init(&barr, NULL, PROCESSOR_NUM))
	{
		printf("Could not creat a barrier\n");
		return -1;
	}	

	// Initialize the global array
	for (i = 0; i < ARRAY_ROW_NUM; i++)
	{
		for (j = 0; j < ARRAY_COL_NUM; j++)
		{
			global_array[i][j].counter = 0;
			for (k = 0; k < ITER_NUM; k++)
			{
				global_array[i][j].iter_list[k] = 0;
			}
		}
	}	
	
	// Begin to process the raw data and find all two-item pair
	pthread_mutex_init(&mutexsum, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (i = 0; i < PROCESSOR_NUM; i++)
	{
		if (pthread_create(&thread[i], &attr, find_large_item, (void *)((long) i)))
		{
			printf("Could not create thread %d\n", i);
			return -1;
		}
	}
	
	pthread_attr_destroy(&attr);

	for (i = 0; i < PROCESSOR_NUM; i++)
	{
		if (pthread_join(thread[i], &status))
		{
			printf("Could not join thread %d\n", i);
			return -1;
		}
	}
		
	pthread_mutex_destroy(&mutexsum);
	
	// Debug code
	if (DEBUG_2)
	{	
		printf("Global Counter & List: \n");
		for (i = 0; i < ARRAY_ROW_NUM; i++)
		{
			for (j = 0; j < ARRAY_COL_NUM; j++)
			{
				if (global_array[i][j].counter)
				{
					printf("Combination '%c%c' happen %d times at iteration: \n", i + 97, j + 97, global_array[i][j].counter);
					for (k = 0; k < ITER_NUM; k++)
					{
						if (global_array[i][j].iter_list[k])
						{
							printf("%d\t", k);
						}
					}	
					printf("\n");
				}
			}
			printf("\n");
		}
	}
	
	pthread_exit(NULL);
}



	
