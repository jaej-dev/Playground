/*
  Question 17:
  Recursive function for factorial
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int factorial(int num)
{
  if (num == 1)
    return 1;
  else
    return (num * factorial(num-1));
}

int main(void)
{
  int n;
  int result;

  printf("Press number for factorial: ");
  scanf("%d", &n);

  result = factorial(n);
  printf("Result = %d\n", result); 

  return 0;
}


