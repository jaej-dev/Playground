#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char num_to_string(int val)
{
  char buf[10];
  if (val > 9 || val < 1)
    return '1';

  sprintf(&buf, "%d", val);

  return buf[0];
}

char* string_compress(char* str)
{
  size_t str_len;
  char p = '\0';
  int i = 1, j = 0, count = 1;
  char num[10];
  char result[20] = "";
  char result_count = 0;

  str_len = strlen(str);
  result[j] = str[0];

  for (i = 0; i < str_len; i++)
  {
    if (str[i] == p)
    {
      count++;
      continue;
    }
    else
    {
      result[j++] = str[i];
      result[j++] = num_to_string(count);
      p = str[i+1];
      count = 1;
      result_count += 2;
    }
    printf("%s: count = %d, i = %d, j = %d, p = %c\n", __func__, count , i, j, p);
  }

  if (result_count < str_len)
    return result;
  else
    return str;
}


int main(void)
{
  char* str1 = "aabbcccdddd";
  char* result1 = "";
  char* str2 = "a";
  char* result2 = "";
 
  result1 = string_compress(str1);
  printf("str1 = %s, result1 = %s\n", str1, result1);

  result2 = string_compress(str2);
  printf("str2 = %s, result2 = %s\n", str2, result2);

  return 0;
}
