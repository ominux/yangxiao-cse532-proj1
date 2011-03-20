// Global Variables
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define CHAR_NUM_MAX    26	
#define ARRAY_ROW_NUM	26
#define ARRAY_COL_NUM	26
#define LIST_SIZE       1000
// DEBUG LEVEL
#define DEBUG_1			1	// Display read in data array
#define DEBUG_2			0	// Display global_array
#define DEBUG_3			0	// Display local_array counter
#define DEBUG_4			0	// Display local_array detail computation, seldom use, need to combine with Thread ID when use
#define DEBUG_5			0 	// Display itemset output info

// Global Variables
pthread_mutex_t mutexsum;



// Input needed global variables
int ITER_NUM;
int CHAR_NUM;
int PROCESSOR_NUM;
int THRESHOLD;
int MAX_ITEM_SIZE;
int BARRIER_LMT;
int barrier_counter;

char original_array[100][CHAR_NUM_MAX + 1];
typedef struct
{
	int	counter;
	int	iter_list[100];
} CELLDATA;
CELLDATA 	global_array[ARRAY_ROW_NUM][ARRAY_COL_NUM];

typedef struct
{
	CELLDATA stats;
	int items[CHAR_NUM_MAX];
} LIST;





// Function declearation
void *find_large_item(void *arg);


int main(void)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int threshold = 0;
	pthread_attr_t attr;
    pthread_t thread[PROCESSOR_NUM];

	void *status;
	
	// ------------------------------
	// Initialize global
	// ------------------------------
	printf("Enter ITER_NUM: \n");
	scanf("%d", &ITER_NUM);

	printf("Enter CHAR_NUM: \n");
	scanf("%d", &CHAR_NUM);

	printf("Enter PROCESSOR_NUM: \n");
	scanf("%d", &PROCESSOR_NUM);

	printf("Enter THRESHOLD: \n");
	scanf("%d", &THRESHOLD);

	MAX_ITEM_SIZE	= CHAR_NUM;
	BARRIER_LMT		= PROCESSOR_NUM;
	barrier_counter = 0;	// clear the counter

	// Generate raw data array
	for (i = 0; i < ITER_NUM; i++)
	{
		for (j = 0; j < CHAR_NUM_MAX; j++)
		{
			tag_list[j] = 0;
		}
		char_num = rand() % CHAR_NUM + 1;
		original_array[i][0] = char_num;
		//fprintf(fp, "%d\t", char_num);
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
			orginal_array[i][j] = rand_char;
		}
	}
	
	// Verify the raw data array
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

		
	if (DEBUG_1 || DEBUG_2 || DEBUG_3)
	{	
		printf("ITER_NUM: %d, PROCESSOR_NUM: %d\n", ITER_NUM, PROCESSOR_NUM);	
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
		printf("main thread: global counter & list\n");
		for (i = 0; i < ARRAY_ROW_NUM; i++)
        {
        	for (j = 0; j < ARRAY_COL_NUM; j++)
			{
				if (global_array[i][j].counter)
				{
					printf("Combination '%c%c' happen %d times at iteration: ", i + 97, j + 97, global_array[i][j].counter);
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
		}
	}
	
	pthread_exit(NULL);
}



	
