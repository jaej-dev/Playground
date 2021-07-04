/*           
 * Title        :   04. Prototype
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "ColourManager.h"

using namespace std;


/*
 * main entry
 */  
int main (void)
{
  cout << "\nmain: create colour manager ------------------------------" << endl;
  ColourManager* colourManager = new ColourManager();

  cout << "\nmain: add simple colours ---------------------------------" << endl;
  colourManager->AddColour("Red", new Colour(255, 0, 0));
  colourManager->AddColour("Green", new Colour(0, 255, 0));
  colourManager->AddColour("Blue", new Colour(0, 0, 255));

  cout << "\nmain: add complex colours --------------------------------" << endl;
  colourManager->AddColour("Angry", new Colour(255, 54, 0));
  colourManager->AddColour("Peace", new Colour(128, 211, 128));
  colourManager->AddColour("Flame", new Colour(211, 34, 20));

  //Clone existing colours, modify and add them to the manager
  cout << "\nmain: 1st clone exsting colours --------------------------" << endl;
  ColourPrototype* colour1 = (colourManager->GetColour("Red"))->Clone();
  (dynamic_cast<Colour*>(colour1))->SetRed(200);
  colourManager->AddColour("Light Red", colour1);

  cout << "\nmain: 2nd clone exsting colours --------------------------" << endl;
  ColourPrototype* colour2 = (colourManager->GetColour("Peace"))->Clone();
  (dynamic_cast<Colour*>(colour2))->SetRed(150);
  (dynamic_cast<Colour*>(colour2))->SetBlue(150);
  colourManager->AddColour("Extreme Peace", colour2);

  cout << "\nmain: 3th clone exsting colours --------------------------" << endl;
  ColourPrototype* colour3 = (colourManager->GetColour("Flame"))->Clone();
  (dynamic_cast<Colour*>(colour3))->SetRed(255);
  colourManager->AddColour("Hot Flame", colour3);

  cout << "\nmain: print colours --------------------------------------" << endl;
  colourManager->PrintColours();

  //clean the memory
  cout << "\nmain: clean memory ---------------------------------------" << endl;
  delete colourManager;

  cout << "\nmain: program exit..." << endl;

  return 0;
}

