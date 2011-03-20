#ifndef __GlobalVar_h__
#define __GlobalVar_h__
	// Libraries
	#include <pthread.h>
	#include <stdlib.h>
	#include <stdio.h>

	// Global variables declearation
	#define CHAR_NUM_MAX    26	// Maximum char number for each iteration
	#define ARRAY_ROW_NUM	26
	#define ARRAY_COL_NUM	26
	#define LIST_SIZE       1000
	// DEBUG LEVEL
	#define DEBUG_1			0	// Display read in data array
	#define DEBUG_2			0	// Display global_array
	#define DEBUG_3			0	// Display local_array counter
	#define DEBUG_4			0	// Display local_array detail computation, seldom use, need to combine with Thread ID when use
	#define DEBUG_5			1 	// Display itemset output info

	// Global Variables
	pthread_mutex_t mutexsum;
	//pthread_barrier_t barr; 	//barrier variable for all threads to synchronize on global_array



	// Input needed global variables
	int ITER_NUM;
	int CHAR_NUM;
	int PROCESSOR_NUM;
	int THRESHOLD;
	int MAX_ITEM_SIZE;
	int BARRIER_LMT;
	int barrier_counter;	//barrier counter used for sync all the threads.

	char 		original_array[100][CHAR_NUM_MAX + 1];
	typedef struct
	{
		int		counter;
		int		iter_list[100];
	} CELLDATA;
	CELLDATA 	global_array[ARRAY_ROW_NUM][ARRAY_COL_NUM];
	
	typedef struct
	{
		CELLDATA stats;
		int items[CHAR_NUM_MAX];
	} LIST;

	void print_ARRAY(CELLDATA array[ARRAY_ROW_NUM][ARRAY_COL_NUM]);

#endif
