/*           
 * Title        :   01. Abstract Factory
 * Filename     :   Game.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#pragma once 
#include "IRace.h"

class Game
{
public:
  Game();
  Game(IRace* race);
  ~Game();
  void Show(void);

private:
  IMaincenterBuilding * _maincenter;
  IDepotBuilding * _depot;
};

