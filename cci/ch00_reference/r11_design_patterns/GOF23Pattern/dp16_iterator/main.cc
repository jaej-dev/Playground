/*           
 * Title        :   16. Iterator pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Iterator.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create vending machine aggregate -----------------" << endl;
  VendingMachineAggregate* myVendingMachine = new VendingMachineAggregate;
  myVendingMachine->AddDrink("Pepsi");
  myVendingMachine->AddDrink("Cider");
  myVendingMachine->AddDrink("Fanta");
  
  cout << "\nmain: Remove drink -------------------------------------" << endl;
  myVendingMachine->RemoveDrink("Fanta");

  cout << "\nmain: Print all drink list -----------------------------" << endl;
  IIterator* iterator = myVendingMachine->CreateIterator();
  while (iterator->IsDone())
  {
    Drink* myDrinkList = (Drink*)iterator->Next();
    cout << "main: " << myDrinkList->GetName() << endl;
  }

  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete myVendingMachine;

  return 0;
}
