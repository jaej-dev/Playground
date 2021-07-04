/*           
 * Title        :   Arrays and Strings Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 2. Reverse
 * Implement a function void reverse(char* str) in C or C++ which reverses a nullterminated string.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void reverse(char *str) {
	char* end = str;
	char tmp;

	if (!str)
		return;

	while (*end) {
		++end;
	}
	--end;
		
	while (str < end) {
		tmp = *str;
		*str++ = *end;
		*end-- = tmp;
	}
}


int main(void)
{
#if 0
  char str[100];	// ok
	printf("Enter string: ");
	gets(str);
#endif

#if 1
	char str[10] = "123456789";	// ok
#endif

#if 0
	char * str = "abc";	// it make a segmentation fault
#endif

	printf("Input a stirng: %s\n", str);

  reverse(str);

	printf("Reverse of entered string: %s\n", str);

  return 0;
}
