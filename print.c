#include "GlobalVar.h"

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
