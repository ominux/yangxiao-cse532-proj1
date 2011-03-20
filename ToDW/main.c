// Global Variables
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define LISTSIZE       1000
// DEBUG LEVEL
#define DEBUG_1			1	// Display read in data array
#define DEBUG_2			0	// Display wholedata
#define DEBUG_3			0	// Display local_array counter
#define DEBUG_4			0	// Display local_array detail computation, seldom use, need to combine with Thread ID when use
#define DEBUG_5			1 	// Display itemset output info

pthread_mutex_t thread_lock;



// Input needed global variables
int xfernum;
int itemnum;
int procnum;
int THRESHOLD;
int mis;
int bcmax;
int bc;

char rawdata[100][26 + 1];
typedef struct
{
	int	counter;
	int	xferrec[100];
} cell;

cell wholedata[26][26];

typedef struct
{
	cell stats;
	int items[26];
} list;


// Function declearation
void *large(void *arg);
void showarray(cell array[26][26]);

int main(void)
{
	int		i = 0;
	int		j = 0;
	int		k = 0;
	int		tag_list[26];
	int		realitemnum;

	char 	tempitem	= ' ';
	
	pthread_attr_t attr;
    pthread_t thread[procnum];

	void *status;
	
	// ------------------------------
	// Initialize global
	// ------------------------------
	/*
	printf("Enter xfernum: \n");
	scanf("%d", &xfernum);

	printf("Enter itemnum: \n");
	scanf("%d", &itemnum);

	printf("Enter procnum: \n");
	scanf("%d", &procnum);

	printf("Enter THRESHOLD: \n");
	scanf("%d", &THRESHOLD);
	*/
	xfernum 	= 20;
	itemnum		= 5;
	procnum		= 5;
	THRESHOLD	= 1;


	mis		= itemnum;
	bcmax	= procnum;
	bc 		= 0;	

	// Generate raw data array
	for (i = 0; i < xfernum; i++)
	{
		for (j = 0; j < 26; j++)
		{
			tag_list[j] = 0;
		}
		realitemnum = rand() % itemnum + 1;
		rawdata[i][0] = realitemnum;
		for (j = 1; j < realitemnum + 1; j++)
		{
			int temp;
			do
			{
				temp = rand() % 26;
			}
			while (tag_list[temp]);
			tag_list[temp] = 1;	
			tempitem = (char) (temp + 97);
			rawdata[i][j] = tempitem;
		}
	}
	
	// Verify the raw data array
	/*for (i = 0; i < xfernum; i++)
	{
		realitemnum = rawdata[i][0];
		printf("%d\t", realitemnum);
		for (j = 1; j < realitemnum + 1; j++)
		{   
			printf("%c\t", rawdata[i][j]);
		}   
		printf("\n");   
	}*/

	// Initialize the global array
	for (i = 0; i < 26; i++)
	{
		for (j = 0; j < 26; j++)
		{
			wholedata[i][j].counter = 0;
			for (k = 0; k < xfernum; k++)
			{
				wholedata[i][j].xferrec[k] = 0;
			}
		}
	}	
	
	// Begin to process the raw data and find all two-item pair
	pthread_mutex_init(&thread_lock, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	for (i = 0; i < procnum; i++)
	{
		if (pthread_create(&thread[i], &attr, large, (void *)((long) i)))
		{
			printf("THREAD CREATION ERROR! %d\n", i);
			return 0;
		}
	}
	pthread_attr_destroy(&attr);

	for (i = 0; i < procnum; i++)
	{
		if (pthread_join(thread[i], &status))
		{
			printf("THREAD JOIN ERROR! %d\n", i);
			return 0;
		}
	}
		
	pthread_mutex_destroy(&thread_lock);
	
	// Debug code
	if (DEBUG_2)
	{	
		printf("main thread: global counter & list\n");
		showarray(wholedata);
	}
	
	pthread_exit(NULL);
}



void *large(void *arg)
{
	int 	i, j, k, s, p;
	int		start, end, flag, count;
	int 	threadid, realitemnum;
    int 	next_index = 0;
    int 	tmp[xfernum];
    int 	index_i = 0;
    int 	index_j = 0;
	int 	index = 0;
	cell 	local_array[26][26];
	list	curr_itemset[LISTSIZE];
	list	next_itemset[LISTSIZE];
		
	threadid 	= (int)((long) arg);
	start 	= threadid * xfernum / procnum;	
	end		= (threadid + 1) * xfernum / procnum;
	
	
	// Initialzie local_array, clear counter and iteration list
	for (i = 0; i < 26; i++)
	{
		for (j = 0; j < 26; j++)
		{
			local_array[i][j].counter = 0;
			for (k = 0; k < xfernum; k++)
			{
				local_array[i][j].xferrec[k] = 0;
			}
		}
	}	
	
	// Scan the raw data from the start iter to end iter
	if (DEBUG_4)
	{
		printf("Thread ID: %d Local Array --> Detail:\n", threadid);
		printf("Start: %d, End: %d\n", start, end);
	}
	
	// Begin to process data
	for (i = start; i < end; i++)
	{
		realitemnum = rawdata[i][0];
		if (DEBUG_4)
		{
			printf("Char_num: %d\n", realitemnum);
		}
		for (j = 1; j < realitemnum + 1; j++)
		{
			if (DEBUG_4)
			{
				printf("I: %d, J: %d, Original_Array[i][j]: %c\n", i, j, rawdata[i][j]);
			}
			for (k = j + 1; k < realitemnum + 1; k++)
			{
				if (rawdata[i][j] < rawdata[i][k])
				{
					index_i = rawdata[i][j] - 97;
					index_j = rawdata[i][k] - 97;
					local_array[index_i][index_j].counter++;
					local_array[index_i][index_j].xferrec[i] = 1;
				}
				else if (rawdata[i][j] > rawdata[i][k])
				{
					index_i = rawdata[i][k] - 97;
					index_j = rawdata[i][j] - 97;
					local_array[index_i][index_j].counter++;
					local_array[index_i][index_j].xferrec[i] = 1;
				}
	
				if (DEBUG_4)
				{
					printf("Original_Array[i][k]: %c, Index_I: %d, Index_J: %d, Couter: %d, List: %d\n", rawdata[i][k], index_i, index_j, local_array[index_i][index_j].counter, local_array[index_i][index_j].xferrec[i]);
				}
			}
		}
	}
	
	// Merge the local data to global data
	pthread_mutex_lock(&thread_lock);

	for (i = 0; i < 26; i++)
	{
		for (j = 0; j < 26; j++)
		{
			wholedata[i][j].counter = wholedata[i][j].counter + local_array[i][j].counter;
			for (k = start; k < end; k++)
			{
				wholedata[i][j].xferrec[k] = local_array[i][j].xferrec[k];
			}
		}
	}

	// Debud code
	if (DEBUG_3)
	{
		printf("Thread ID: %d Local Counter & List: \n", threadid);
		printf("Start: %d, End: %d\n", start, end);
		showarray(local_array);
	}
	bc++;
	
	pthread_mutex_unlock(&thread_lock);

	// Wait for global data renewed
	while (bc < bcmax)
	{
	}
	
	if (DEBUG_2)
	{
		pthread_mutex_lock(&thread_lock);
		printf("After sync\n");
        printf("Thread ID: %d Local Counter & List: \n", threadid);
        printf("Start: %d, End: %d\n", start, end);
        showarray(wholedata);
		pthread_mutex_unlock(&thread_lock);
	}
	
	start   = threadid * 26 / procnum;
    end     = (threadid + 1) * 26 / procnum;
	if (DEBUG_3)
	{
		printf("Thread ID: %d, start = %d, end = %d \n", threadid, start, end);
	}
	for (i = start; i < end; i++)
	{
		for (j = 0; j < 26; j++)
		{
			if (wholedata[i][j].counter >= THRESHOLD)
			{
				curr_itemset[index].items[0] = i;
				curr_itemset[index].items[1] = j;
				curr_itemset[index].stats.counter = wholedata[i][j].counter;
				for (k = 0; k < xfernum; k++)
				{
					curr_itemset[index].stats.xferrec[k] = wholedata[i][j].xferrec[k];
				}
			 	index++;
			}
		}
	}
	
	if (DEBUG_5)
	{
		printf("Thread ID = %d, large itemset of size 2: \n", threadid);
		for (i = 0; i < index; i++)
		{
			printf("%c%c ",curr_itemset[i].items[0]+97, curr_itemset[i].items[1]+97);
		}
		printf("\n");
	}
	
	for (s = 1; s < mis -1; s++)
	{
		for (i = 0; i < index-1; i++)
		{
			for (j = i+1; j < index; j++)
			{
				flag = 1;
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
					break; 
				} 
				else
				{
					count = 0;
					for (k = 0; k < xfernum; k++)
					{
						tmp[k] = 0;
					}
					for (k = 0; k < xfernum; k++)
					{
						if (curr_itemset[i].stats.xferrec[k] == curr_itemset[j].stats.xferrec[k])
						{
							if (curr_itemset[i].stats.xferrec[k] > 0)
							{
								count++;
								tmp[k] = 1;
							}
						}
					}
					if (count >= THRESHOLD)
					{
						next_itemset[next_index].stats.counter = count;
						for (k = 0; k < xfernum; k++)
						{
							next_itemset[next_index].stats.xferrec[k] = tmp[k];
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
		if (DEBUG_5)
		{
   			printf("Thread ID = %d, large itemset of size %d: \n", threadid, s+2);
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

		index = next_index;
		for (i = 0; i < next_index; i++)
		{
			curr_itemset[i].stats.counter = next_itemset[i].stats.counter;
			for (k = 0; k < xfernum; k++)
			{
				curr_itemset[i].stats.xferrec[k] = next_itemset[i].stats.xferrec[k];
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
	
void showarray(cell array[26][26])
{
	int i, j, k;
	
	for (i = 0; i < 26; i++)
	{
		for (j = 0; j < 26; j++)
		{
			if (array[i][j].counter)
			{
				printf("Combination '%c%c' happen %d times at iteration: ", i + 97, j + 97, array[i][j].counter);
				for (k = 0; k < xfernum; k++)
				{
					if (array[i][j].xferrec[k])
					{
						printf("%d\t", k);
					}
				}
				printf("\n");
			}
		}
	}
}
