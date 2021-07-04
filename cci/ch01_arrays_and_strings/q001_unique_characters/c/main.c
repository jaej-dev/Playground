#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int is_unique_chars(char* str)
{
  int checker = 0;
  int i;
  int val;
  size_t strsize = strlen(str);

  printf("%s: str = %s, size = %d\n", __func__, str, strsize);

  for (i = 0; i < strsize; i++)
  {
    val = str[i] - 'a';

    if ((checker & (1 << val)) > 0)
      return 0;
    
    checker |= (1 << val);
    printf("%s: bit vector = 0x%08x\n", __func__, val);
  }
  return 1;
}

int main (void)
{
  char* str1 = "abcdefg";
  char* str2 = "abcdefga";

  if (is_unique_chars(str1))
    printf("str1 = true\n");
  else
    printf("str1 = false\n");

  if (is_unique_chars(str2))
    printf("str2 = true\n");
  else
    printf("str2 = false\n");
}

