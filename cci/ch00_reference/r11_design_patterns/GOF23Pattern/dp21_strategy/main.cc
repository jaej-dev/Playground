/*           
 * Title        :   21. Strategy pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Strategy.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create contact data ------------------------------" << endl;
  ContactData data = { "James", "Dean", "123-456-789", "James@email.com" };
  
  cout << "\nmain: Stream record algorithm --------------------------" << endl;
  StreamRecord streamRec(std::cout);
  ContactRecorder contact1(&streamRec);
  contact1.Store(data);

  cout << "\nmain: Database record algorithm ------------------------" << endl;
  DatabaseRecord dbRec;
  ContactRecorder contact2(&dbRec);
  contact2.Store(data);

  cout << "\nmain: Program exit -------------------------------------" << endl;

  return 0;
}
