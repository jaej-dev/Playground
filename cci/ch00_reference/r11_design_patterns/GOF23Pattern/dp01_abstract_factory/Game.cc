/*           
 * Title        :   01. Abstract Factory
 * Filename     :   Game.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include "Game.h"

Game::Game()
{
  std::cout << "Game::Game: done" << std::endl;
}

Game::Game(IRace* race)
{
  _maincenter = race->CreateMaincenterBuilding();
  _depot = race->CreateDepotBuilding();
  std::cout << "Game::Game: race = " << race << ", done"<< std::endl;
}

Game::~Game()
{
  std::cout << "Game::~Game: done" << std::endl;
}

void Game::Show(void)
{
  std::cout << "Game::Show: _maincenter = " << _maincenter << std::endl;
  std::cout << "Game::Show: _depot      = " << _depot << std::endl;
}

