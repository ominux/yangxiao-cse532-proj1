#ifndef __GlobalVar_h__
#define __GlobalVar_h__
	// Libraries
	#include <pthread.h>
	#include <stdlib.h>
	#include <stdio.h>

	// Global variables declearation
	#define ITER_NUM        100
	#define CHAR_NUM_MAX    100
	#define PROCESSOR_NUM   5
	char original_array[ITER_NUM][CHAR_NUM_MAX + 1];
	int global_counter_array[26][26];
	int global_array[5][5];
#endif
