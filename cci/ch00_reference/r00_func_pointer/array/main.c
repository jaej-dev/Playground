/*           
 * Title        :   Data Structure Example
 * Filename     :   main.c
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 0. Function pointer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int* func()
{
  int dummy[2] = {1,2};
  return dummy;
}

int main(void)
{
  int * p = func();


	printf("1st = %d, 2nd = %d\n", p[0], p[1]);

  return 0;
}
