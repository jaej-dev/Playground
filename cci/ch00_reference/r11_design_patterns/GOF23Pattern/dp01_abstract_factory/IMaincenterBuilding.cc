/*           
 * Title        :   01. Abstract Factory
 * Filename     :   IMaincenterBuilding.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include "ConcreteFactory.h"

IMaincenterBuilding::IMaincenterBuilding()
{
  std::cout << "IMaincenterBuilding::IMaincenterBuilding: done" << std::endl;
}

IMaincenterBuilding::~IMaincenterBuilding()
{
  std::cout << "IMaincenterBuilding::~IMaincenterBuilding: done" << std::endl;
}

