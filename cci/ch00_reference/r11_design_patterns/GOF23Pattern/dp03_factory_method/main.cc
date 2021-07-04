/*           
 * Title        :   03. Factory Method
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "ConcreteDocument.h"

using namespace std;


/*
 * main entry
 */  
int main (void)
{
  cout << "main: create document" << endl;
  Document * doc1 = new Report();
  Document * doc2 = new Resume();

  //Get and print the pages of the second document
  list<IPage*>& doc1Pages = const_cast<list<IPage*>&> (doc1->GetPages());
  cout << "main: Report Pages -------------" << endl;
  for(list<IPage*>::iterator it = doc1Pages.begin(); it != doc1Pages.end(); it++)
  {
    cout << "\t" << (*it)->GetPageName() << endl;
  }

  list<IPage*>& doc2Pages = const_cast<list<IPage*>&> (doc2->GetPages());
  cout << "main: Resume Pages -------------" << endl;
  for(list<IPage*>::iterator it = doc2Pages.begin(); it != doc2Pages.end(); it++)
  {
    cout << "\t" << (*it)->GetPageName() << endl;
  }

  cout << "main: delete document" << endl;
  delete doc1;
  delete doc2;
  cout << "main: program exit..." << endl;

  return 0;
}
     
       
          

