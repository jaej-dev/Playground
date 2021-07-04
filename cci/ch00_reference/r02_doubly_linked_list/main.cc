/*           
 * Title        :   Data Structure Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 2. Doubly Linked List by cpp
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "doublylist.h"

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
  int test2[5] = { 600,700,800,900,1000 }; 
  DoublyLinkedList<int> list;

  cout << "Step 1: Add some nodes by front" << endl;
  for ( size_t i = 0; i < 5; ++i)
    list.AppendNodeFront(test1[i]);

  cout << "Step 2: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  cout << "Step 3: Add some nodes by back" << endl;
  for ( size_t i = 0; i < 5; ++i)
    list.AppendNodeBack(test2[i]);

  cout << "Step 4: Print list by backward (Tail to Head)" << endl;
  list.PrintListBackward();

  cout << "Step 5: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  cout << "Step 6: Delete node" << endl;
  list.DeleteNode(999); // just test
  list.DeleteNode(500);  // Head
  list.DeleteNode(1000); // Tail
  list.DeleteNode(100);
  list.DeleteNode(200);
  list.DeleteNode(111); // just test

  cout << "Step 7: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  cout << "Step 8: Clear list" << endl;
  list.ClearList();

  cout << "Step 9: Print list by forward (Head to Tail)" << endl;
  list.PrintListForward();

  return 0;
}
