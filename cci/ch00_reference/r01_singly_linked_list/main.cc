/*           
 * Title        :   Data Structure Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 1.Singly Linked List by cpp
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "singlylist.h"

using namespace std;



int main (void)
{
  int test[10] = { 100,200,300,400,500,600,700,800,100,777 }; 
  SinglyLinkedList<int> list;

  cout << "Step 1: Add some nodes" << endl;
  for ( size_t i = 0; i < 10; ++i)
    list.AppendNode(test[i]);

  cout << "Step 2: Print all nodes" << endl;
  list.PrintList();

  cout << "Step 3: Delete some nodes" << endl;
  list.DeleteNode(100);
  list.DeleteNode(200);
  list.DeleteNode(111);

  cout << "Step 4: Print all nodes" << endl;
  list.PrintList();
  
  cout << "Step 5: Delete all nodes" << endl;
  list.ClearList();

  cout << "Step 6: Print all nodes" << endl;
  list.PrintList();

  return 0;
}
