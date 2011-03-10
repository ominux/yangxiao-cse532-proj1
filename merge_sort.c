#include "GlobalVar.h"

void merge(char *array_pt, int low_bound, int middle_bound, int up_bound)
{
	char temp_1[100];
	char temp_2[100];
	int i, j, k;

  	for(i = 0; i < middle_bound - low_bound + 1; i++)
	{
    	temp_1[i] = array_pt[low_bound+i];
	}
	temp_1[i] = 126; 
  	
	for(j = 0; j < up_bound - middle_bound; j++)
	{
    	temp_2[j] = array_pt[middle_bound + 1 + j];
	}
	temp_2[j] = 126;

  	i = 0;
  	j = 0;
  	for(k = low_bound; k <= up_bound; k++)
    {
      	if(temp_1[i] < temp_2[j])
		{
	  		array_pt[k] = temp_1[i];
	  		i++;
		}
      	else if(temp_1[i] >= temp_2[j])
		{
	  		array_pt[k] = temp_2[j];
	  		j++;
		}
    }
}

void merge_sort(char *array_pt, int low_bound, int up_bound)
{
	int middle_bound;
	/*int i;
	for (i = low_bound; i <= up_bound; i++)
	{
		printf("%c\t", array_pt[i]);
	}
	printf("\n");*/

  	if (low_bound < up_bound)
    {
      middle_bound = (low_bound + up_bound) / 2;
      merge_sort(array_pt, low_bound, middle_bound);
      merge_sort(array_pt, middle_bound + 1, up_bound);
      merge(array_pt, low_bound, middle_bound, up_bound);
    }
}
