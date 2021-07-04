/*
  Question 7:
  Quiz

 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int main(void)
{
#if 0
  int a = 1, *b;
  b = &a;
  printf("size of a = %d, size of b = %d\n", (int)sizeof(a), (int)sizeof(b));
#endif

#if 0
  printf("1\n");
  fork();
  printf("2\n");
  fork();
  printf("3\n");
#endif

#if 0
  int i, j;
  for(i=0,j=0;i<10,j<5;i++,j++)
  {
    printf("i=%d \t j=%d\n", i, j);
  }
#endif

#if 1
  int a=2;
  if(a==2)
  {
    printf("%d\n", ~a);
    a=~a+2<<1;
    printf("%d\n",a);
  }
  else
  {
    a=~a;
  }
#endif

  return 0;
}


