/*           
 * Title        :   20. State pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "State.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create context -----------------------------------" << endl;
  Account* account = new Account("Dr. Who");

  cout << "\nmain: Start banking ------------------------------------" << endl;
  account->Withdraw(10.00);
  account->Withdraw(30.00);
  account->Withdraw(70.00);
  account->Deposit(234.00);
  account->Deposit(5000.00);
  account->Withdraw(5200.00);
  account->Deposit(1500.00);
  account->Deposit(1.00);
  account->Withdraw(1200.00);

  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete account;

  return 0;
}
