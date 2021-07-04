/*           
 * Title        :   13. Chain of responsibility pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Chain_of_responsibility.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create modules and make chain --------------------" << endl;
  ModuleEntry* m = new ModuleEntry(EventHandler::E_NONE);
  Wifi* w = new Wifi(m, EventHandler::E_WIFI);
  Lte* l = new Lte(w, EventHandler::E_NONE);
  Bt* b = new Bt(l, EventHandler::E_NONE);
  Nfc* n = new Nfc(b, EventHandler::E_NFC);

  cout << "\nmain: Handle event  ------------------------------------" << endl;
  n->HandleEvent();

  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete n;
  delete b;
  delete l;
  delete w;
  delete m;

  return 0;
}
