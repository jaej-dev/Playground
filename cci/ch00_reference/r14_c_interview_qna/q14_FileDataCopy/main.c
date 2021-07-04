/*
  Question 14:
  Copy data from one file to another file
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int main(void)
{
  FILE *a, *b;
  char* name_a = "file1.txt";
  char* name_b = "file2.txt";
  char ch;
  
  a = fopen(name_a, "r");
  b = fopen(name_b, "w");
  if (a == NULL || b == NULL)
  {
    printf("Can not open file\n");
    return 0;
  }

  while((ch = getc(a)) != EOF) {
      putc(ch, b);
  }

  fclose(a);
  fclose(b);
  printf("program exit\n");

  return 0;
}


