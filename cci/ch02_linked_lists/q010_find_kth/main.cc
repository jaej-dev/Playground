/*           
 * Title        :   Chapter2 Linked Lists
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from /cci/Ch00_Reference/r01_singly_linked_list.
 * 
 * Question 10: find kth
 * Implement an algorithm to find the kth to last element of 
 * a singly linked list. 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "singlylist.h"

using namespace std;

int main (void)
{
  int test[10] = { 100,200,300,400,500,600,700,800,900,1000 }; 
  SingleLinkedList<int> list;

  cout << "Step 1: Add some nodes" << endl;
  for ( size_t i = 0; i < 10; ++i)
    list.AppendNode(test[i]);

  cout << "Step 2: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 3: Find kTh to last element" << endl;
  int k = 7;
  cout << k << "th to last element = " << list.FindKthToLast(k) << endl;

  cout << "Step 7: Delete all nodes" << endl;
  list.ClearList();

  cout << "Step 8: Print all nodes" << endl;
  list.PrintList();

  return 0;
}
