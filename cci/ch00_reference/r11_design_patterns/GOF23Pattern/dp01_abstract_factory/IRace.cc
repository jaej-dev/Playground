/*           
 * Title        :   01. Abstract Factory
 * Filename     :   IRace.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include <string>
#include "IRace.h"

IRace::IRace()
{
  std::cout << "IRace::IRace: done" << std::endl;
}

IRace::~IRace()
{
  std::cout << "IRace::~IRace: done" << std::endl;
}

IMaincenterBuilding * IRace::CreateMaincenterBuilding(void)
{
  std::cout << "IRace::CreateMaincenterBuilding: done" << std::endl;
  return NULL;
}

IDepotBuilding * IRace::CreateDepotBuilding(void)
{
  std::cout << "IRace::CreateDepotBuilding: done" << std::endl;
  return NULL;
}

