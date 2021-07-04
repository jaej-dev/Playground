/*
  Question 18:
  Find the size of int without using sizeof operator
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int main(void)
{
#if 0
  int *n = 0;
  n++;
  printf("Size of int = %d\n", n);
#endif
#if 0
  float *n = 0;
  n++;
  printf("Size of float = %d\n", n);
#endif
#if 1
  double *n = 0;
  n++;
  printf("Size of double = %d\n", n);
#endif

  return 0;
}


