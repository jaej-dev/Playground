/*           
 * Title        :   02. Builder
 * Filename     :   IHouseBuilder.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "IHouseBuilder.h"

IHouseBuilder::IHouseBuilder()
{
  std::cout << "IHouseBuilder::IHouseBuilder: done." << std::endl;
}
IHouseBuilder::~IHouseBuilder()
{
  std::cout << "IHouseBuilder::~IHouseBuilder: done." << std::endl;
}

void IHouseBuilder::BuildWindow(void)
{
  std::cout << "IHouseBuilder::BuildeWindow: done." << std::endl;
}

void IHouseBuilder::BuildDoor(void)
{
  std::cout << "IHouseBuilder::BuildeDoor: done." << std::endl;
}

void IHouseBuilder::BuildFloor(void)
{
  std::cout << "IHouseBuilder::BuildeFloor: done." << std::endl;
}

House* IHouseBuilder::GetHouse(void)
{
  std::cout << "IHouseBuilder::GetHouse: done." << std::endl;
}
