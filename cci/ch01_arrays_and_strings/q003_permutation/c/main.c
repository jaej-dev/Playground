#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int is_permutation(char* str1, char* str2)
{
  int letters[256] = { 0, };
  size_t i;
  size_t str1_len;
  size_t str2_len;
  char temp;
 
  str1_len = strlen(str1);
  str2_len = strlen(str2);


  for (i = 0; i < str1_len; i++)
  {
    temp = str1[i]; printf("%d : %d\n", i, temp);
    letters[(int)temp]++;
  }

  for (i = 0; i < str2_len; i++)
  {
    temp = str2[i]; printf("%d : %d\n", i, temp);
    letters[(int)temp]--;

    if (letters[temp] < 0)
      return 0;
  }
  return 1;
}

int main(void)
{
  /*
  char str1[5] = "abcde";
  char str2[5] = "abced";
  char str3[5] = "abcdf";
  */
  char* str1 = "abcde";
  char* str2 = "abced";
  char* str3 = "abcdf";

  /* true case */
  if (is_permutation(str1, str2))
    printf("case 1 is ture, str1 = %s, str2 = %s\n", str1, str2);
  else
    printf("case 1 is false, str1 = %s, str2 = %s\n", str1, str2);

  /* false case*/
  if (is_permutation(str1, str3))
    printf("case 2 is ture, str1 = %s, str3 = %s\n", str1, str3);
  else
    printf("case 2 is false, str1 = %s, str3 = %s\n", str1, str3);

  return 0;
}
