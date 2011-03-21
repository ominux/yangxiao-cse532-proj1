// Global Variables
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define LISTSIZE       1000

pthread_mutex_t thread_lock;

// Input needed global variables
int xfernum;
int itemnum;
int procnum;
int THRESHOLD;
int mis;
int bcmax;
int bc;

int rawdata[100][30 + 1];
typedef struct
{
	int	counter;
	int	xferrec[100];
} cell;

cell wholedata[30][30];

typedef struct
{
	cell stats;
	int items[30];
} list;


// Function declearation
void *large(void *arg);

int main()
{
	pthread_attr_t attr;
    pthread_t thread[procnum];
	void *status;

	int		i = 0, j = 0, k = 0;
	int		tag_list[30];
	int		realitemnum = 0;
	// ------------------------------
	// Initialize global
	// ------------------------------
	
	printf("Enter xfernum, iternum, procnum, THRESHOLD: \n");
	scanf("%d, %d, %d, %d", &xfernum, &itemnum, &procnum, &THRESHOLD);

	//printf("%d, %d, %d, %d\n", xfernum, itemnum, procnum, THRESHOLD);
	mis		= itemnum;
	bcmax	= procnum;
	bc 		= 0;	

	// Generate raw data array
	for (i = 0; i < xfernum; i++)
	{
		for (j = 0; j < 30; j++)
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
				temp = rand() % 30;
			}
			while (tag_list[temp]);
			tag_list[temp] = 1;	
			rawdata[i][j] = temp;
		}
	}

	// Initialize the global array
	for (i = 0; i < 30; i++)
	{
		for (j = 0; j < 30; j++)
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
	pthread_exit(NULL);
}



void *large(void *arg)
{
	int 	i, j, k, s, p;
	int		start, end, flag, count;
	int 	threadid, realitemnum;
    int 	next_index = 0;
    int 	tmp[xfernum];
    int 	index_i = 0, index_j = 0, index = 0;
	cell 	local_array[30][30];
	list	curr_itemset[LISTSIZE];
	list	next_itemset[LISTSIZE];
		
	threadid 	= (int)((long) arg);
	start 	= threadid * xfernum / procnum;	
	end		= (threadid + 1) * xfernum / procnum;
	
	
	// Initialzie Internal array
	for (i = 0; i < 30; i++)
	{
		for (j = 0; j < 30; j++)
		{
			local_array[i][j].counter = 0;
			for (k = 0; k < xfernum; k++)
			{
				local_array[i][j].xferrec[k] = 0;
			}
		}
	}	
	
	// Begin to process data
	for (i = start; i < end; i++)
	{
		realitemnum = rawdata[i][0];
		for (j = 1; j < realitemnum + 1; j++)
		{
			for (k = j + 1; k < realitemnum + 1; k++)
			{
				if (rawdata[i][j] < rawdata[i][k])
				{
					index_i = rawdata[i][j];
					index_j = rawdata[i][k];
					local_array[index_i][index_j].counter++;
					local_array[index_i][index_j].xferrec[i] = 1;
				}
				else if (rawdata[i][j] > rawdata[i][k])
				{
					index_i = rawdata[i][k];
					index_j = rawdata[i][j];
					local_array[index_i][index_j].counter++;
					local_array[index_i][index_j].xferrec[i] = 1;
				}
			}
		}
	}
	
	// Merge to global 
	pthread_mutex_lock(&thread_lock);
	for (i = 0; i < 30; i++)
	{
		for (j = 0; j < 30; j++)
		{
			wholedata[i][j].counter = wholedata[i][j].counter + local_array[i][j].counter;
			for (k = start; k < end; k++)
			{
				wholedata[i][j].xferrec[k] = local_array[i][j].xferrec[k];
			}
		}
	}
	bc++;
	pthread_mutex_unlock(&thread_lock);

	// Wait 
	while (bc < bcmax)
	{
	}
	
	start   = threadid * 30 / procnum;
    end     = (threadid + 1) * 30 / procnum;
	
	for (i = start; i < end; i++)
	{
		for (j = 0; j < 30; j++)
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
	