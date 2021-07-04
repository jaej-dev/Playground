/*
   http://www.crazyforcode.com/c-programming/

   Question 1:
   Given a positive integer n, count the total number of set bits in binary
   representation of all numbers from 1 to n

  Examples:
  Input: n = 3
  Output: 4

  Input: n = 6
  Output: 9
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int get_set_bits(unsigned int n)
{
  unsigned int r, i;

  for (r = 0; n; n--)
  {
    for (i = n; i; i >>= 1)
    {
      r = r + (i & 1);  /* check set bit */
#if 0
      printf("%s: n = %d, i = %d, result = %d\n", __func__, n, i, r);
#endif
    }
  }

  return r;
}

int main(void)
{
  int n;
  
  printf("Press positive interger: ");
  scanf("%d", &n);
  printf("N = %d, Count of set bits = %d\n", n, get_set_bits(n));

#if 1
  n = 1;
  printf("N = %d, Count of set bits = %d\n", n, get_set_bits(n));
  
  n = 3;
  printf("N = %d, Count of set bits = %d\n", n, get_set_bits(n));
  
  n = 6;
  printf("N = %d, Count of set bits = %d\n", n, get_set_bits(n));
  
  n = 7;
  printf("N = %d, Count of set bits = %d\n", n, get_set_bits(n));

  n = 8;
  printf("N = %d, Count of set bits = %d\n", n, get_set_bits(n));
#endif

  return 0;
}
