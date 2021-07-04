/*           
 * Title        :   06. Adapter
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Adapter.h"


using namespace std;


/*
 * main entry
 * client
 */  
int main (void)
{
  cout << "\nmain: create client instance ---------------------------" << endl;

  SwitchBoard *mySwitchBoard = new SwitchBoard; // Adaptee
   /* Target = Adapter(Adaptee) */
  AbstractPlug *adapter = new Adapter(mySwitchBoard);
  
  cout << "\nmain: execute client by adpatee ------------------------" << endl;
  adapter->RoundPin();
  adapter->PinCount();

  delete adapter;
  delete mySwitchBoard;
  cout << "\nmain: program exit -------------------------------------" << endl;

  return 0;
}

