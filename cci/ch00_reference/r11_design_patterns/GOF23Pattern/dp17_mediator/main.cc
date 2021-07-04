/*           
 * Title        :   17. Mediator pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Mediator.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create mediator and colleage A, N ---------------------" << endl;
  ConcreteMediator* mediator = new ConcreteMediator();
  Colleage* c1 = new ConcreteColleageA(mediator);
  Colleage* c2 = new ConcreteColleageN(mediator);

  cout << "\nmain: Set mediator for each colleage -------------------" << endl;
  mediator->SetColleageA(c1);
  mediator->SetColleageN(c2);

  cout << "\nmain: Send message to each colleage --------------------" << endl;
  c1->SendMsg("Message from colleage A");
  c2->SendMsg("Message from colleage B");

  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete mediator;
  delete c1;
  delete c2;

  return 0;
}
