/*           
 * Title        :   01. Abstract Factory
 * Filename     :   BuildingProducts.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include <string>
#include "BuildingProducts.h"

CommandCenter::CommandCenter()
{
  std::cout << "CommandCenter::CommandCenter: done" << std::endl;
}

CommandCenter::~CommandCenter()
{
  std::cout << "CommandCenter::~CommandCenter: done" << std::endl;
}

SupplyDepot::SupplyDepot()
{
  std::cout << "SupplyDepot::SupplyDepot: done" << std::endl;
}

SupplyDepot::~SupplyDepot()
{
  std::cout << "SupplyDepot::~SupplyDepot: done" << std::endl;
}

void SupplyDepot::Interact(IMaincenterBuilding b)
{
  std::cout << "SupplyDepot::Interact: done" << std::endl;
}

Nexus::Nexus()
{
  std::cout << "Nexus::Nexus: done" << std::endl;
}

Nexus::~Nexus()
{
  std::cout << "Nexus::~Nexus: done" << std::endl;
}

Pylon::Pylon()
{
  std::cout << "Pylon::Pylon: done" << std::endl;
}

Pylon::~Pylon()
{
  std::cout << "Pylon::~Pylon: done" << std::endl;
}

void Pylon::Interact(IMaincenterBuilding b)
{
  std::cout << "Pylon::Interact: done" << std::endl;
}
