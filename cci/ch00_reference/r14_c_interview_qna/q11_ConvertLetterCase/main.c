/*
  Question 11:
  Conversion between uppercase and lower case
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


char* convert_case(char* str, int to_upper)
{
  char result[100] = "";
  int i = 0;

  while(*str) {

    if (!to_upper)
    {
      if (*str >= 65 && *str <= 90)
        result[i] = *str + 32;
      else
        result[i] = *str;
    }
    else
    {
      if (*str >= 97 && *str <= 122)
        result[i] = *str - 32;
      else
        result[i] = *str;
    }
#if 0
    printf("str[%d] = %c, result = %s\n", i, *str, result);
#endif
    str++;
    i++;
  }

  return result;
}

int main(void)
{
  char str[100] = "";
  char* result = "";

  printf("Press any word: ");
  scanf("%s", str);

  result = convert_case(str, 0);
  printf("Lower case result = %s\n", result);

  result = convert_case(str, 1);
  printf("Upper case result = %s\n", result);

  return 0;
}


