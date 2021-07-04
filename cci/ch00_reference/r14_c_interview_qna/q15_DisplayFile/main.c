/*
  Question 15:
  Copy data from one file to another file
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


int main(void)
{
  FILE *fp;
  char* file_name = "file1.txt";
  char ch;
  
  fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    printf("Can not open file\n");
    return 0;
  }

  while((ch = getc(fp)) != EOF) {
      putchar(ch);
  }

  fclose(fp);
  printf("program exit\n");

  return 0;
}


