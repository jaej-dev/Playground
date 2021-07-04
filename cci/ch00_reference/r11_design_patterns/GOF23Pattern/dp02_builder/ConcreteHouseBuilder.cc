/*           
 * Title        :   02. Builder
 * Filename     :   ConcreteHouseBuilder.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "ConcreteHouseBuilder.h"

LavishHouse::LavishHouse(void)
{
  _house = new House();
  std::cout << "LavishHouse::LavishHouse: done." << std::endl;
}

LavishHouse::~LavishHouse(void)
{
    delete _house;
  std::cout << "LavishHouse::~LavishHouse: done." << std::endl;
}
       
void LavishHouse::BuildWindow(void)
{
  _house->SetWindow("Lavish Window");
  std::cout << "LavishHouse::BuilWindow: done." << std::endl;
}
  
void LavishHouse::BuildDoor(void)
{
  _house->SetDoor("Lavish Door");
  std::cout << "LavishHouse::BuilDoor: done." << std::endl;
}

void LavishHouse::BuildFloor(void)
{
  _house->SetFloor("Lavish Floor");
  std::cout << "LavishHouse::BuilFloor: done." << std::endl;
}
            
House* LavishHouse::GetHouse(void)
{
  House* house = this->_house;
  std::cout << "LavishHouse::GetHouse: done. house = " << house << std::endl;
  return house;
}

NormalHouse::NormalHouse(void)
{
  _house = new House();
  std::cout << "NormalHouse::NormalHouse: done." << std::endl;
}

NormalHouse::~NormalHouse(void)
{
  delete _house;
  std::cout << "NormalHouse::~NormalHouse: done." << std::endl;
}
       
void NormalHouse::BuildWindow(void)
{
  _house->SetWindow("Normal Window");
  std::cout << "NormalHouse::BuilWindow: done." << std::endl;
}
        
void NormalHouse::BuildDoor(void)
{
  _house->SetDoor("Normal Door");
  std::cout << "NormalHouse::BuilDoor: done." << std::endl;
}
         
void NormalHouse::BuildFloor(void)
{
  _house->SetFloor("Normal Floor");
  std::cout << "NormalHouse::BuilFloor: done." << std::endl;
}
            
House* NormalHouse::GetHouse(void)
{
  House* house = this->_house;
  std::cout << "NormalHouse::GetHouse: done. house = " << house << std::endl;
  return house;
}
