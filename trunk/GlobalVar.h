#ifndef __GlobalVar_h__
#define __GlobalVar_h__
	// Libraries
	#include <pthread.h>
	#include <stdlib.h>
	#include <stdio.h>

	// Global variables declearation
	#define ITER_NUM        10
	#define CHAR_NUM_MAX    26
	#define PROCESSOR_NUM   5
	#define ARRAY_ROW_NUM	26
	#define ARRAY_COL_NUM	26
	#define DEBUG_1			1	// Display local_array counter
	#define DEBUG_2			0	// Display local_array iter_list
	#define DEBUG_3			0	// Display local_array detail computation
	#define DEBUG_4			1	// Display read in data array
	#define DEBUG_5			1	// Display global_array
	pthread_mutex_t mutexsum;
	char 		original_array[ITER_NUM][CHAR_NUM_MAX + 1];
	typedef struct
	{
		int		counter;
		int		iter_list[ITER_NUM];
	} CELLDATA;
	CELLDATA 	global_array[ARRAY_ROW_NUM][ARRAY_COL_NUM];
#endif
