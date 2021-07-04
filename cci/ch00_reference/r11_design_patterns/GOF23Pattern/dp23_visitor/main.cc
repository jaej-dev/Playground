/*           
 * Title        :   23. Visitor pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Visitor.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create car with some parts -----------------------" << endl;
  Car* car = new Car("MyCar");
  Body* body = new Body("SuperBody", 1);
  Engine* engine = new Engine("TurboEngine", 0);
  car->AddPart(body);
  car->AddPart(engine);

  cout << "\nmain: Create inspector ---------------------------------" << endl;
  CarPartVisitor* inspector = new CarPartInspector();

  cout << "\nmain: Start inspecttion --------------------------------" << endl;
  car->Accept(inspector);

  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete car;
  delete body;
  delete engine;
  delete inspector;

  return 0;
}
