/*           
 * Title        :   01. Abstract Factory
 * Filename     :   IDepotBuilding.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include <string>
#include "IDepotBuilding.h"

IDepotBuilding::IDepotBuilding()
{
  std::cout << "IDepotBuilding::IDepotBuidling: done" << std::endl;
}

IDepotBuilding::~IDepotBuilding()
{
  std::cout << "IDepotBuilding::~IDepotBuidling: done" << std::endl;
}

void IDepotBuilding::Interact(IMaincenterBuilding b)
{
  std::cout << "IDepotBuilding::Interact: done" << std::endl;
}
