/*
  Question 19:
  Bubble sort
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

int* bubble_sort(int* arr, int arr_size)
{
  int i, j, temp;

  for (i = arr_size - 2; i >= 0; i--)
  {
    for (j = 0; j <= i; j++)
    {
      if (arr[j] > arr[j+1])
      {
        temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
  }

  return arr;
}

int main(void)
{
  int arr[10] = { 7, 1, 3, 9, 4, 6, 2, 5, 8, 0 };
  int * ret;
  int i;

  printf("Origin array:\n");
  for (i = 0; i < 10; i++)
    printf("%d ", arr[i]);

  printf("\nSorted array:\n");
  ret = bubble_sort(arr, 10);
  for (i = 0; i < 10; i++)
    printf("%d ", ret[i]);
  printf("\n");
  
  return 0;
}


