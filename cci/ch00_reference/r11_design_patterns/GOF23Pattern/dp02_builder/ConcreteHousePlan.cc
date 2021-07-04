/*           
 * Title        :   02. Builder
 * Filename     :   ConcreteHousePlan.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "ConcreteHousePlan.h"

House::House()
{
  std::cout << "House::House: done." << std::endl;
}
House::~House()
{
  std::cout << "House::~House: done." << std::endl;
}

void House::SetWindow(std::string window)
{
  this->_window = window;
  std::cout << "House::SetWindow: window = " << this->_window << std::endl;
}

void House::SetDoor(std::string door)
{
  this->_door = door;
  std::cout << "House::SetDoor: door = " << this->_door << std::endl;
}

void House::SetFloor(std::string floor)
{
  this->_floor = floor;
  std::cout << "House::SetFloor: floor = " << this->_floor << std::endl;
}
 
