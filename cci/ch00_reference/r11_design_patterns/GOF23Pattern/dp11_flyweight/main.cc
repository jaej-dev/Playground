/*           
 * Title        :   11. Flyweight pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Flyweight.h"

using namespace std;

/*
 * main entry
 * client
 */  
int main(void)
{
  cout << "\nmain: Create CharacterFactory --------------------------" << endl;
  string document = "ABNABN";
  const char* chars = document.c_str();
  CharacterFactory* factory = new CharacterFactory;

  cout << "\nmain: Create every character use a flyweight object ----" << endl;
  // extrinsic state
  int pointSize = 10;

  // For each character use a flyweight object
  for(size_t i = 0; i < document.length(); i++)
  {
    pointSize++;
    Character* character = factory->GetCharacter(chars[i]);
    character->Display(pointSize);
  }

  //Clean memory
  cout << "\nmain: Delete every character use a flyweight object ----" << endl;
  delete factory; 
  
  cout << "\nmain: Program exit -------------------------------------" << endl;
  
  return 0;
}
