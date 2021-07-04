/*
  Question 8:
  Reverse any number

 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

int reverse_num(int x)
{
  int result, temp;

  while(x) {
    temp = x % 10;
    result = (result * 10) + temp;
    x = x / 10;
  }

  return result;
}


int main(void)
{
  int x;

  printf("Press any number: ");
  scanf("%d", &x);
  printf("%d's reverse number = %d\n", x, reverse_num(x));

  return 0;
}


