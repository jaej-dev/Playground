/*           
 * Title        :   Chapter2 Linked Lists
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from /cci/Ch00_Reference/r01_singly_linked_list.
 * 
 * Question 11: Delete node
 * Implement an algorithm to delete a node in the middle of 
 * a singly linked list, given only access to that node.
 *
 * EXAMPLE
 * Input: the node c from the linked list a->b->c->d->e
 * Result: nothing is returned, but the new linked list looks like a- >b- >d->e
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
  int test[10] = { 100,200,300,400,500,600,700,800,900,1000 }; 
  SingleLinkedList<int> list;

  cout << "Step 1: Add some nodes" << endl;
  for ( size_t i = 0; i < 10; ++i)
    list.AppendNode(test[i]);

  cout << "Step 2: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 3: Delete node by node" << endl;
  SingleNode<int> * n;
  n = list.GetHead();  // for only test
  n = n->GetNext();
  cout << "target delete node addr= " << n << ", data= " << n->GetData() << endl;
  list.DeleteNodeByNode(n);

  cout << "Step 4: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 5: Delete all nodes" << endl;
  list.ClearList();

  cout << "Step 6: Print all nodes" << endl;
  list.PrintList();

  return 0;
}
