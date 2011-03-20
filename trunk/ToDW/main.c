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
#define DEBUG_5			1 	// Display itemset output info

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
void print_ARRAY(CELLDATA array[ARRAY_ROW_NUM][ARRAY_COL_NUM]);

int main(void)
{
	int		i = 0;
	int		j = 0;
	int		k = 0;
	int		threshold = 0;
	int		tag_list[CHAR_NUM_MAX];
	int		char_num;

	char 	rand_char	= ' ';
	
	pthread_attr_t attr;
    pthread_t thread[PROCESSOR_NUM];

	void *status;
	
	// ------------------------------
	// Initialize global
	// ------------------------------
	/*
	printf("Enter ITER_NUM: \n");
	scanf("%d", &ITER_NUM);

	printf("Enter CHAR_NUM: \n");
	scanf("%d", &CHAR_NUM);

	printf("Enter PROCESSOR_NUM: \n");
	scanf("%d", &PROCESSOR_NUM);

	printf("Enter THRESHOLD: \n");
	scanf("%d", &THRESHOLD);
	*/
	ITER_NUM 		= 20;
	CHAR_NUM		= 5;
	PROCESSOR_NUM	= 5;
	THRESHOLD		= 1;


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
		for (j = 1; j < char_num + 1; j++)
		{
			int temp;
			do
			{
				temp = rand() % 26;
			}
			while (tag_list[temp]);
			tag_list[temp] = 1;	
			rand_char = (char) (temp + 97);
			original_array[i][j] = rand_char;
		}
	}
	
	// Verify the raw data array
	printf("READ IN ARRAY:\n");
	for (i = 0; i < ITER_NUM; i++)
	{
		char_num = original_array[i][0];
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
		print_ARRAY(global_array);
	}
	
	pthread_exit(NULL);
}



void *find_large_item(void *arg)
{
	int 	i;
	int		j;
	int		k;
	int		start;
	int		end;
	int 	tid;
	int		char_num;
	int 	s,p;
    int 	flag;
    int 	next_index = 0;
    int 	count;
    int 	tmp[ITER_NUM];
    int 	index_i = 0;
    int 	index_j = 0;
	int 	index = 0;


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
	// -----------------------------
	// Increase barrier counter
	// -----------------------------
	barrier_counter++;
	//printf("barrier_counter: %d\n", barrier_counter);
	
	pthread_mutex_unlock(&mutexsum);
	
	// Synchronization point: wait for all peers to finish updating global_array 
	//int sync = pthread_barrier_wait(&barr);


	// -----------------------------
	// Wait for barrier counter
	// -----------------------------
	while (barrier_counter < BARRIER_LMT)
	{
	}
	//printf("out of loop! thread id = %d \n", tid);
	/*if (sync != 0 && sync != PTHREAD_BARRIER_SERIAL_THREAD)
	{
		printf("Could not wait on barrier\n");
		exit(-1);
	}*/
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
	if (DEBUG_5)
	{
		printf("Thread ID = %d, large itemset of size 2: \n", tid);
		for (i = 0; i < index; i++)
		{
			printf("%c%c ",curr_itemset[i].items[0]+97, curr_itemset[i].items[1]+97);
		}
		printf("\n");
	}
	// Each thread generates large itemset of size s (s>=3) and print them out
	for (s = 1; s < MAX_ITEM_SIZE -1; s++)
	{
		for (i = 0; i < index-1; i++)
		{
			for (j = i+1; j < index; j++)
			{
				flag = 1;
				// compare the first s characters and they have to match to be candidates
				for (p = 0; p < s; p++)
				{
					if(curr_itemset[i].items[p] != curr_itemset[j].items[p])
					{
						flag = 0;
						break;
					}
				}
				if (!flag)
				{
					break; // provided that the lists are sorted alphabetically
				} 
				else
				{
					// find a candidate, now checks the transaction lists to see if they have enough appearances in common
					count = 0;
					for (k = 0; k < ITER_NUM; k++)
					{
						tmp[k] = 0;
					}
					for (k = 0; k < ITER_NUM; k++)
					{
						if (curr_itemset[i].stats.iter_list[k] == curr_itemset[j].stats.iter_list[k])
						{
							if (curr_itemset[i].stats.iter_list[k] > 0)
							{
								count++;
								tmp[k] = 1;
							}
						}
					}
					if (count >= THRESHOLD)
					{
						next_itemset[next_index].stats.counter = count;
						for (k = 0; k < ITER_NUM; k++)
						{
							next_itemset[next_index].stats.iter_list[k] = tmp[k];
						}
						for(p = 0; p <=s; p++)
						{
							next_itemset[next_index].items[p] = curr_itemset[i].items[p];
						}
						next_itemset[next_index].items[s+1] = curr_itemset[j].items[s];
						next_index++;
					}							
				}
			}
		}
	    // print out large itemset of size s+2
		if (DEBUG_5)
		{
   			printf("Thread ID = %d, large itemset of size %d: \n", tid, s+2);
    		for (i = 0; i < next_index; i++)
    		{
        		for(j = 0; j < s+2; j++)
				{
					printf("%c",next_itemset[i].items[j]+97);
				}
				printf(" ");
    		}
    		printf("\n");
		}

		// copy next_itemset to curr_itemset and starts the next round
		index = next_index;
		for (i = 0; i < next_index; i++)
		{
			curr_itemset[i].stats.counter = next_itemset[i].stats.counter;
			for (k = 0; k < ITER_NUM; k++)
			{
				curr_itemset[i].stats.iter_list[k] = next_itemset[i].stats.iter_list[k];
			}
			for (k = 0; k < s+2; k++)
			{
				curr_itemset[i].items[k] = next_itemset[i].items[k];
			}
		}
		next_index = 0;
	}
	pthread_exit((void*) 0);
}
	
void print_ARRAY(CELLDATA array[ARRAY_ROW_NUM][ARRAY_COL_NUM])
{
	int i, j, k;
	
	for (i = 0; i < ARRAY_ROW_NUM; i++)
	{
		for (j = 0; j < ARRAY_COL_NUM; j++)
		{
			if (array[i][j].counter)
			{
				printf("Combination '%c%c' happen %d times at iteration: ", i + 97, j + 97, array[i][j].counter);
				for (k = 0; k < ITER_NUM; k++)
				{
					if (array[i][j].iter_list[k])
					{
						printf("%d\t", k);
					}
				}
				printf("\n");
			}
		}
	}
}
