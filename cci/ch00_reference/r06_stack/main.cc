/*           
 * Title        :   Data Structure Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 6. Stack by cpp
 * 
 * Track and memory check : 
 * clear && make && valgrind --track-origins=yes --leak-check=yes ./run
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "stack.h"

using namespace std;



int main (void)
{
  int test[10] = { 100,200,300,400,500,600,700,800,900,777 }; 
  DynamicStack<int> stack;

  cout << "Step 1: Add some nodes into stack" << endl;
  for ( size_t i = 0; i < 10; ++i)
    stack.Push(test[i]);

  cout << "Step 2: Print all nodes" << endl;
  stack.PrintStack();

  cout << "Step 3: Pop some nodes" << endl;
  stack.Pop();
  stack.Pop();
  stack.Pop();
#if 1
  // over pop test
  stack.Pop();  stack.Pop();  stack.Pop();  stack.Pop();  stack.Pop();
  stack.Pop();  stack.Pop();  stack.Pop();  stack.Pop();  stack.Pop();
  stack.Pop();  stack.Pop();  stack.Pop();  stack.Pop();  stack.Pop();
#endif

  cout << "Step 4: Print all nodes" << endl;
  stack.PrintStack();

#if 0  
  cout << "Step 5: Delete all nodes" << endl;
  stack.ClearStack();

  cout << "Step 6: Print all nodes" << endl;
  stack.PrintStack();
#else
  // destroyer test
  cout << "Step 6: program exit" << endl;
#endif

  return 0;
}
