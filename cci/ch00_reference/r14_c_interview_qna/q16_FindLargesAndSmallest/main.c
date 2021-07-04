/*
  Question 16:
  Find largest and smallest number in an array 
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


void find_min_max(int* num, int arr_size, int* min, int* max)
{
  int i;
  *min = *num;
  *max = *num;
  
  for (i = 0; i < arr_size; i++)
  {
    if (num[i] < *min)
      *min = num[i];
    if (num[i] > *max)
      *max = num[i];
    printf("%s: min = %d, max = %d\n", __func__, *min, *max);
  }
}

int main(void)
{
  int num[10] = { -1, 2, -3, 4, -5, 6, - 7, 8, -9, 10 };
  int max, min;

  find_min_max(num, 10, &min, &max);

  printf("min = %d, max = %d\n", min, max);
  return 0;
}


