/*
  Question 12:
  Concatenation of two strings
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


char* concatenation(char* str1, char* str2)
{
  char result[100] = "";
  int i = 0;
  
  if (!*str1 || !*str2)
    return NULL;

  while (*str1) {
    result[i++] = *str1;
    str1++;
  }

  while (*str2) {
    result[i++] = *str2;
    str2++;
  }

  printf("result = %s\n", result);

  return result;
}

int main(void)
{
  char str1[50] = "";
  char str2[50] = "";
  char* result = "";

  printf("Press first word: ");
  scanf("%s", str1);

  printf("Press second word: ");
  scanf("%s", str2);

  result = concatenation(str1, str2);
  printf("Concatenation = %s\n", result);

  return 0;
}


