/*
  Question 3:
  Find the Element that Appears Once

  Problem:
  Given an array where every element occurs three times,
  except one element which occurs only once.
  Find the element that occurs once.
  Expected time complexity is O(n) and O(1) extra space.

  Examples:
  Input: arr[] = {10, 1, 10, 3, 10, 1, 1, 2, 3, 3}
  Output: 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int find_unique_element(int* arr)
{
  int ones = 0;
  int twos = 0;
  int not_trees = 0;

  while (*arr) {
    twos |= ones & *arr;
    ones ^= *arr;
    not_trees = ~(ones & twos);

    ones &= not_trees;
    twos &= not_trees; 
    printf("%s: arr = %d, ones = %d, twos = %d, nor_trees = %d\n",
      __func__, *arr, ones, twos, not_trees);
    arr++;
  }
  return ones;
}

int main(void)
{
  int arr[10] = { 1,1,1, 2,2,2, 3,3,3, 4 };

  printf("uinque elements = %d\n", find_unique_element(arr));

  return 0;
}



