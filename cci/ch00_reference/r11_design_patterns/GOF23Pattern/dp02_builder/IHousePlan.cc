/*           
 * Title        :   02. Builder
 * Filename     :   IHousePlan.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "IHousePlan.h"

IHousePlan::IHousePlan()
{
  std::cout << "IHousePlan::IHousePlan: done." << std::endl;
}

IHousePlan:: ~IHousePlan()
{
  std::cout << "IHousePlan::~IHousePlan: done." << std::endl;
}

void IHousePlan::SetWindow(std::string window)
{
  std::cout << "IHousePlan::SetWindow: done." << std::endl;
}

void IHousePlan::SetDoor(std::string door)
{
  std::cout << "IHousePlan::SetDoor: done." << std::endl;
}

void IHousePlan::SetFloor(std::string floor)
{
  std::cout << "IHousePlan::SetFloor: done." << std::endl;
}
 
