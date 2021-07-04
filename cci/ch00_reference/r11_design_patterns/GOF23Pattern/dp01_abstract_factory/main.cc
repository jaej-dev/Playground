/*           
 * Title        :   01. Abstract Factory
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "ConcreteFactory.h"
#include "Game.h"

using namespace std;


/*
 * main entry
 */  
int main (void)
{
  cout << "main: create race 1" << endl;
  IRace * race1 = new Terran();

  cout << "main: create game 1" << endl;
  Game * game1 = new Game(race1);
  game1->Show();

  cout << "main: create race 2" << endl;
  IRace * race2 = new Protoss();

  cout << "main: create game 2" << endl;
  Game * game2 = new Game(race2);
  game2->Show();

  cout << "main: delete games 1" << endl;
  delete game1;

  cout << "main: delete race 1" << endl;
  delete race1;

  cout << "main: delete games 2" << endl;
  delete game2;
  
  cout << "main: delete race 2" << endl;
  delete race2;
  
  cout << "main: program exit..." << endl;
  return 0;
}

