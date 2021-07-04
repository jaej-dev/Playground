/*           
 * Title        :   09. Decorator
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Decorator.h"

using namespace std;

/*
 * main entry
 * client
 */  
int main(void)
{
  cout << "\nmain: create desktop -----------------------------------" << endl;
  Computer* d1 = new Desktop(); 
  cout << d1->GetDescription();
  cout << " - total cost = " << d1->GetCost() << std::endl;

  cout << "\nmain: decorated desktop ---------------------------------" << endl;
  Computer* d2 = new Desktop();
  d2 = new Cpu(d2); // CPU changed
  //d2 = new Ram(d2); // RAM changed
  cout << d2->GetDescription();
  cout << " - total cost = " << d2->GetCost() << std::endl;

  cout << "\nmain: create labtop ------------------------------------" << endl;
  Computer* l1 = new Labtop(); 
  cout << l1->GetDescription();
  cout << " - total cost = " << l1->GetCost() << std::endl;

  cout << "\nmain: decorated labtop ---------------------------------" << endl;
  Computer* l2 = new Labtop();
  l2 = new Cpu(l2);
  l2 = new Ram(l2);
  cout << l2->GetDescription();
  cout << " - total cost = " << l2->GetCost() << std::endl;

  delete d1;
  delete d2;
  delete l1;
  delete l2;
  cout << "\nmain: program exit -------------------------------------" << endl;
  
  return 0;
}
