// **************************************************************
// Function: 	main	
// Description:	Used to run the code
// **************************************************************
#include "GlobalVar.h"


// Function declearation
int data_read(void);

pthread_mutex_t mutexsum;



void *find_large_item(void *tid)
{
	int i, j;
	int cell;
	int local_array[5][5];
		
	cell = (int)((long) tid);
	
	printf("Thread ID: %d\n", cell);

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			local_array[i][j] = cell;
		}
	}	
	printf("Thread ID Merge: %d\n", cell);
	
	pthread_mutex_lock(&mutexsum);
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			global_array[i][j] = global_array[i][j] + local_array[i][j];
		}
	}	
	pthread_mutex_unlock(&mutexsum);

	pthread_exit((void*) 0);
}


int main(void)
{
	int i = 0;
	int j = 0;
	int threshold = 0;
	void *status;

	// Read the data from TXT file
	if (data_read() == 0)
	{
		return 0;
	}



	//assoc_gen(threshold);
    pthread_attr_t attr;
	pthread_t thread[PROCESSOR_NUM];
	
	// Initialize the global array
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			global_array[i][j] = 0;
		}
	}	

	pthread_mutex_init(&mutexsum, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (i = 0; i < PROCESSOR_NUM; i++)
	{
		pthread_create(&thread[i], &attr, find_large_item, (void *)((long) i));
	}
	
	pthread_attr_destroy(&attr);

	for (i = 0; i < PROCESSOR_NUM; i++)
	{
		pthread_join(thread[i], &status);
	}
			
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			printf("%d\t", global_array[i][j]);
		}
		printf("\n");
	}	
	pthread_mutex_destroy(&mutexsum);
	pthread_exit(NULL);
}



	
