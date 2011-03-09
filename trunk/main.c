// 
// **************************************************************
// Function: 	main	
// Description:	Used to run the code
// **************************************************************
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define PROCESSOR_NUM 10

int assoc_gen(int threshold);
void *print_message_function( void *ptr );

int main(void)
{
	int i = 0;
	int threshold = 0;
	assoc_gen(threshold);
     
	

	pthread_t thread[PROCESSOR_NUM];
	
	for (i = 0; i < PROCESSOR_NUM; i++)
	{
		pthread_create( &thread[i], NULL, print_message_function, ref);

	}

     exit(0);
}


void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     printf("%s \n", message);
}

	
