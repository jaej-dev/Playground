/*
  Question 13:
  Create a file, write text, and close
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int main(void)
{
  FILE* fp;
  char ch;

  fp = fopen("test.txt", "w");
  printf("Write your own word: ");
  while ((ch = getchar()) != EOF) {
    putc(ch, fp);
  }
  fclose(fp);
  printf("program exit\n");
  return 0;
}


