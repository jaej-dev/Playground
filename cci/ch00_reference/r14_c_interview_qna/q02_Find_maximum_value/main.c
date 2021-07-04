/*
   Question 2:
   Find the Maximum of Two Numbers Without Using if-else

   Find the maximum and minimum of two integers without branching
   i.e. if condition.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int min(int x, int y)
{
 return y^((x^y) & -(x<y));
}

int max(int x, int y)
{
 return x^((x^y) & -(x<y));
}

int main(void)
{
  int x = 1, y = 2;
  printf("x = %d, y = %d, min = %d, max = %d\n", x, y, min(x,y), max(x,y));

  x = -1; y = -2;
  printf("x = %d, y = %d, min = %d, max = %d\n", x, y, min(x,y), max(x,y));

  return 0;
}
