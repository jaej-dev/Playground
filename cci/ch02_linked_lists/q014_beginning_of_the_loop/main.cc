/*           
 * Title        :   Chapter2 Linked Lists Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from 
 * /cci/Ch00_Reference/r03_circular_doubly_linked_list.
 * 
 * Question 14: beginning of the loop
 * Given a circular linked list, implement an algorithm which returns the node 
 * at the beginning of the loop.
 *
 * DEFINITION
 * Circular linked list: A (corrupt) linked list in which a node's next pointer 
 * points to an earlier node, so as to make a loop in the linked list.
 *
 * EXAMPLE
 * Input: A - > B - > C - > D - > E - > C [the same C as earlier]
 * Output: C
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "circulardoublylist.h"

using namespace std;



/*
 *              5   4   3   2   1
 * step1: HEAD 500 400 300 200 100 TAIL
 *              6   7   8   9   10
 * step3: HEAD 600 700 800 900 1000 TAIL
 */  
int main (void)
{
  int test1[5] = { 100,200,300,400,500 }; 
  CircularDoublyLinkedList<int> list;

  cout << "Step 1: Add some nodes by front" << endl;
  for ( size_t i = 0; i < 5; ++i)
    list.AppendNodeFront(test1[i]);

  cout << "Step 2: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  cout << "Step 3: Find beginning loop: " << endl;
  cout << "result node addr = " << list.FindBeginningLoop() << endl;

  cout << "Step 4: Clear list" << endl;
  list.ClearList();

  cout << "Step 5: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  return 0;
}
