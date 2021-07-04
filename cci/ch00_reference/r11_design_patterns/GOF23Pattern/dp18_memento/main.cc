/*           
 * Title        :   18. Memento pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Memento.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create caretaker and originator ------------------" << endl;
  Caretaker caretaker;
  Originator* originator = new Originator();
  
  cout << "\nmain: Store some state to memento ----------------------" << endl;
  originator->SetState("action-1");
  caretaker.AddMemento(originator->CreateMemento());
  originator->SetState("action-2");
  caretaker.AddMemento(originator->CreateMemento());
  originator->SetState("action-3");
  caretaker.AddMemento(originator->CreateMemento());
  originator->SetState("action-4");
  caretaker.AddMemento(originator->CreateMemento());
  originator->SetState("action-5");
  caretaker.AddMemento(originator->CreateMemento());
  
  cout << "\nmain: Restore before state from memento ----------------" << endl;
  originator->SetMemento(caretaker.UndoMemento());
  originator->SetMemento(caretaker.UndoMemento());


  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete originator;

  return 0;
}
