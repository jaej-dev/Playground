/*           
 * Title        :   Chapter2 Linked Lists
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from /cci/Ch00_Reference/r01_singly_linked_list.
 * 
 * Question 9: remove duplicates
 * Write code to remove duplicates from an unsorted linked list.
 * FOLLOW UP
 * How would you solve this problem if a temporary buffer is not allowed?
 *
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "singlylist.h"

using namespace std;

int main (void)
{
  int test[10] = { 100,100,200,300,200,400,500,100,600,777 }; 
  SingleLinkedList<int> list;

  cout << "Step 1: Add some nodes" << endl;
  for ( size_t i = 0; i < 10; ++i)
    list.AppendNode(test[i]);

  cout << "Step 2: Print all nodes" << endl;
  list.PrintList();

  cout << "Setp 3: Remove duplicates" << endl;
  list.RemoveDuplicateNode();

  cout << "Step 4: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 5: Delete some nodes" << endl;
  list.DeleteNode(600);

  cout << "Step 6: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 7: Delete all nodes" << endl;
  list.ClearList();

  cout << "Step 8: Print all nodes" << endl;
  list.PrintList();

  return 0;
}
