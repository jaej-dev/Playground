/*
  Question 10:
  Find the largest number

 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int main(void)
{
  int x = 0, y = 0, z = 0;

  for (x = 0; x < 5; x++)
  {
    printf("Press %d's number: ", x);
    scanf("%d", &y);
    if (y > z)
      z = y;
  }

  printf("Largest number = %d\n", z);

  return 0;
}


