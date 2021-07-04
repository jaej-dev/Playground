/*
  Question 2:
  How to Check if a given Number is Fibonacci number?

  In mathematics, the Fibonacci numbers or Fibonacci sequence are the numbers
  in the following integer sequence:

  1,1,2,3,5,8,13,21,34,55,89,144..

  A simple way is to generate Fibonacci numbers until the generated number is
  greater than or equal to 'x'.
  Following is an interesting property about Fibonacci numbers that can also
  be used to check if a given number is Fibonacci or not.
  The question may arise whether a positive integer x is a Fibonacci number.
  This is true if and only if one or both of
  5x^2+4 or 5x^2-4 is a perfect square. (Source: Wiki)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int is_perfect_squre(int x)
{
  int s = sqrt(x);
  return (s * s == x);
}

int is_fibonacci_number(int x)
{
  return (is_perfect_squre(5*x*x + 4) || is_perfect_squre(5*x*x-4));
}

int main(void)
{
  int x[10] = { 0,1,2,3,4,5,6,7,8,9 };
  int y;
  for (y = 0; y < 10; y++)
  {
    printf("%d is%s a fibonacci number\n",
        x[y], is_fibonacci_number(x[y])?"":" not");
  }

  return 0;
}
