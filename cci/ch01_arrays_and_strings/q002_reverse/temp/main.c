#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void reverse(char* str)
{
  char* end = str;
  char tmp;

  printf("end: %p, str: %p\n", end, str);

  if (!str)
    return;
  
  while(*end) {
    ++end;
  }
  --end;

  while(str < end) {
    tmp = *str;
    *str++ = *end;
    *end-- = tmp;
  }
}

int main(void)
{
  char str[6] = "abcdef";
  int a = 10;
 
  printf("Input string: %s\n", str);
  reverse(str);
  printf("Output string: %s\n", str);

  return 0;
}
