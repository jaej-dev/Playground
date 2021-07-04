/*           
 * Title        :   19. Observer pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Observer.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create concrete observers ------------------------" << endl;
  MyObserver observerA("observerA");
  MyObserver observerB("observerB");
  MyObserver observerC("observerC");

  cout << "\nmain: Create concrete subject --------------------------" << endl;
  MySubject subject;
  subject.Subscribe(&observerA);
  subject.Subscribe(&observerB);
  subject.Unsubscribe(&observerB);
  subject.Subscribe(&observerC);

  cout << "\nmain: Notify -------------------------------------------" << endl;
  subject.Notify(MySubject::ADD);
  subject.Notify(MySubject::REMOVE);

  cout << "\nmain: Program exit -------------------------------------" << endl;

  return 0;
}
