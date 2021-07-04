/*           
 * Title        :   01. Abstract Factory
 * Filename     :   ConcreteFactory.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include "ConcreteFactory.h"
#include "BuildingProducts.h"

Terran::Terran()
{
  std::cout << "Terran::Terran: done" << std::endl;
}

Terran::~Terran()
{
  if (_maincenter)
    delete _maincenter;
  if (_depot)
    delete _depot;
  std::cout << "Terran::~Terran: done" << std::endl;
}

IMaincenterBuilding* Terran::CreateMaincenterBuilding(void)
{
  std::cout << "Terran::CreateMaincenterBuilding: done" << std::endl;
  _maincenter = new CommandCenter();
  return _maincenter;
}

IDepotBuilding* Terran::CreateDepotBuilding(void)
{
  std::cout << "Terran::CreateDepotBuilding: done" << std::endl;
  _depot = new SupplyDepot();
  return _depot;
}

Protoss::Protoss()
{
  std::cout << "Protoss::Protoss: done" << std::endl;
}

Protoss::~Protoss()
{
  if (_maincenter)
    delete _maincenter;
  if (_depot)
    delete _depot;
  std::cout << "Protoss::~Protoss: done" << std::endl;
}

IMaincenterBuilding* Protoss::CreateMaincenterBuilding(void)
{
  std::cout << "Protoss::CreateMaincenterBuilding: done" << std::endl;
  _maincenter = new Nexus();
  return _maincenter;
}

IDepotBuilding* Protoss::CreateDepotBuilding(void)
{
  std::cout << "Protoss::CreateDepotBuilding: done" << std::endl;
  _depot = new Pylon();
  return _depot;
}

