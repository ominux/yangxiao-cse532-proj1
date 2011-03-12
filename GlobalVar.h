#ifndef __GlobalVar_h__
#define __GlobalVar_h__
	// Libraries
	#include <pthread.h>
	#include <stdlib.h>
	#include <stdio.h>

	// Global variables declearation
	#define ITER_NUM        5
	#define CHAR_NUM		5	// Customized char number for each iteration
	#define CHAR_NUM_MAX    26	// Maximum char number for each iteration
	#define PROCESSOR_NUM   5
	#define ARRAY_ROW_NUM	26
	#define ARRAY_COL_NUM	26
	
	// DEBUG LEVEL
	#define DEBUG_1			1	// Display read in data array
	#define DEBUG_2			1	// Display global_array
	#define DEBUG_3			1	// Display local_array counter
	#define DEBUG_4			0	// Display local_array detail computation, seldom use, need to combine with Thread ID when use

	// Global Variables
	pthread_mutex_t mutexsum;
	pthread_barrier_t barr; 	//barrier variable for all threads to synchronize on global_array
	char 		original_array[ITER_NUM][CHAR_NUM_MAX + 1];
	typedef struct
	{
		int		counter;
		int		iter_list[ITER_NUM];
	} CELLDATA;
	CELLDATA 	global_array[ARRAY_ROW_NUM][ARRAY_COL_NUM];

	void print_ARRAY(CELLDATA array[ARRAY_ROW_NUM][ARRAY_COL_NUM]);

#endif
