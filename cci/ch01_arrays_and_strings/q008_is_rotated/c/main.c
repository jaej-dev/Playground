#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_substring(char* str1, char* str2)
{
  char * result = strstr(str1, str2);
  if (result == NULL)
    return 0;
  else
    return 1;
}

int is_rotated(char* str1, char* str2)
{
  char str[100] = "";
  char* p = str1;
  char* q = str;

  while(*p)
    *q++  = *p++;
  p = str1;

  while(*p)
    *q++  = *p++;

  q--;
  *q = '\0';
  printf("%s: str = %s\n", __func__, str);

  return is_substring(str, str2);
}

int main(void)
{
  char* str1 = "waterbottle";
  char* str2 = "terbottlewa";
  char* str3 = "terbottleww";
  int ret1 = 0;
  int ret2 = 0;

  ret1 = is_rotated(str1, str2);
  printf("str1 = %s, str2 = %s, result = %d\n", str1, str2, ret1);

  ret2 = is_rotated(str1, str3);
  printf("str1 = %s, str3 = %s, result = %d\n", str1, str3, ret2);

  return 0;
}
