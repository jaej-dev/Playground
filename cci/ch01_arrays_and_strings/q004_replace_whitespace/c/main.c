#include <stdio.h>
#include <stdlib.h>


int get_str_size(char* str)
{
  char* iter = str;
  int str_len = 0, space_len = 0;

  while (*iter) {
    if (*iter == ' ')
      space_len++;
    else
      str_len++;
    iter++;
  }
  return space_len + str_len;
}

int replace_whitespace(char* str, char* result)
{
  int i, j;
  int len = get_str_size(str);

  printf("%s: len = %d\n", __func__, len);

  for (i = 0, j = 0; i < len; i++, j++)
  {
    if (str[i] == ' ')
    {
      result[j] = '%';
      result[++j] = '2';
      result[++j] = '0';
    }
    else
    {
      result[j] = str[i];
    }
    printf("%s: str = %c, %s\n", __func__, str[i], result);
  }
  result[j] = '\0';

  return 0;
}


int main(void)
{
  char* str = "AA BB CC DD EE";
  char result[100] = "";

  replace_whitespace(str, result);

  printf("result = %s, sizeof = %d, strlen = %d\n", result, sizeof(str), strlen(str));

  return 0;
}
