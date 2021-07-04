/*           
 * Title        :   Chapter2 Linked Lists
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from /cci/Ch00_Reference/r01_singly_linked_list.
 * 
 * Question 12: partition list
 * Write code to partition a linked list around a value x, such that all nodes 
 * less than x come before all nodes greater than or equal to x.
 *
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "singlylist.h"

using namespace std;

void PartitionListByNodeData(SingleLinkedList<int> * list, int x)
{
  SingleNode<int> * before = NULL;
  SingleNode<int> * after = NULL;
  SingleNode<int> * runner = list->GetHead();

  if (runner == NULL)
  {
    cout << "PartitionListByNode: empty" << endl;
    return;
  }

  /* 
    Note:
    Insert node into start of before list(or after list)
    Do not insert into end of before list(or after list)!!!
  */
  while (runner != NULL) {
    SingleNode<int> * next = runner->GetNext(); // keep to check next element
    if (runner->GetData() < x)
    {
      if (before == NULL)
      {
        runner->SetNext(NULL);
        before = runner;
      }
      else
      {
        runner->SetNext(before);
        before = runner;
      }
    }
    else
    {
      if (after == NULL)
      {
        runner->SetNext(NULL);
        after = runner;
      }
      else
      {
        runner->SetNext(after);
        after = runner;
      }
    }
#ifdef DEBUG_ENABLED
    cout << "PartitionListByNode: runner= " << runner << ", before= " 
      << before << ", after= " << after << endl;
#endif          
    runner = next;
  } // end of while (runner != NULL)
  
  // merge before and after
  if (before == NULL)
  {
    cout << "PartitionListByNode: nothing has happened" << endl;
  }

  list->SetHead(before);

  while (before->GetNext() != NULL) {
    before = before->GetNext();
  }
  before->SetNext(after);
}


int main (void)
{
  int test[10] = { 600,200,900,400,500,100,700,800,300,1000 }; 
  SingleLinkedList<int> list;

  cout << "Step 1: Add some nodes" << endl;
  for ( size_t i = 0; i < 10; ++i)
    list.AppendNode(test[i]);

  cout << "Step 2: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 3: Partition list by node" << endl;
  cout << "target partition around node data= " << 500 << endl;
  PartitionListByNodeData(&list, 500);

  cout << "Step 4: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 5: Delete all nodes" << endl;
  list.ClearList();

  cout << "Step 6: Print all nodes" << endl;
  list.PrintList();

  return 0;
}
