/*           
 * Title        :   10. Facade
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Facade.h"

using namespace std;

/*
 * main entry
 * client
 */  
int main(void)
{
  cout << "\nmain: Add customer -------------------------------------" << endl;
  Mortgage mortgage;
  Customer* c1 = new Customer("Tester");
  
  cout << "\nmain: Check mortage eligiable --------------------------" << endl;
  bool eligible = mortgage.IsEligible(c1, 7777777);
  cout << "main: " << c1->GetName() << " has been " 
    << (eligible ? "Approved" : "Rejected") << endl;
  
  delete c1;
  cout << "\nmain: Program exit -------------------------------------" << endl;
  
  return 0;
}
