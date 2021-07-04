/*           
 * Title        :   02. Builder
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "Contractor.h"

using namespace std;


/*
 * main entry
 */  
int main (void)
{
  cout << "main: create house builder" << endl;
  IHouseBuilder *lavishHouseBldr = new LavishHouse();
  IHouseBuilder *normalHouseBldr = new NormalHouse();

  cout << "main: create contractor" << endl;
  Contractor *c1 = new Contractor(lavishHouseBldr);
  Contractor *c2 = new Contractor(normalHouseBldr);

  cout << "main: construct house" << endl;
  c1->BuildHouse();
  House *h1 = c1->GetTargetHouse();
  cout << "main: constructed: " << h1 << endl;
  c2->BuildHouse();
  House *h2 = c2->GetTargetHouse();
  cout << "main: constructed: " << h2 << endl;

  cout << "main: delete house builder" << endl;
  delete lavishHouseBldr;
  delete normalHouseBldr;

  cout << "main: delete contractor" << endl;
  delete c1;
  delete c2;

  cout << "main: program exit..." << endl;

  return 0;
}
     
       
          

