/*           
 * Title        :   Chapter2 Linked Lists Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from 
 * /cci/Ch00_Reference/r03_circular_doubly_linked_list.
 * /cci/Ch00_Reference/r06_stack
 * 
 * Question 15: palindrome
 * Implement a function to check if a linked list is a palindrome.
 * 
 * EXAMPLE:
 * 0 -> 1 -> 2 -> 1 -> 0
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "circulardoublylist.h"

using namespace std;

/*
 * main entry
 */  
int main (void)
{
  int test[5] = { 6,7,8,7,6 };  // true case
  //int test[1] = { 7 };  // true case
  //int test[5] = { 2,3,4,5,6 };  // false case
  //int test[6] = { 1,2,3,3,2,1 };  // false case
  CircularDoublyLinkedList<int> list;

  cout << "Step 1: Add some nodes by back" << endl;
  for ( size_t i = 0; i < 5; ++i)
    list.AppendNodeBack(test[i]);

  cout << "Step 2: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  cout << "Step 3: Check palindrom (true case):" << endl;
  cout << "result = " << list.CheckPalindrome() << endl;

  cout << "program exit" << endl;

  return 0;
}
