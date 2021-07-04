/*
  Question 9:
  Program in c to print 1 to 100 without using loop

 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

void print(int x)
{
  static int counter = 1;
  if (x <= 100)
  {
    printf("%d ", x);
    print(++x);
  }
}


int main(void)
{
  int x = 1;
  print(x);

  return 0;
}


