/*           
 * Title        :   02. Builder
 * Filename     :   Contractor.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "Contractor.h"
 
/* The Director. Constructs the house */
Contractor::Contractor()
{
  std::cout << "Contractor::Contractor: done." << std::endl;
}

Contractor::Contractor(IHouseBuilder* housebuilder)
{
  this->_housebuilder = housebuilder;
  std::cout << "Contractor::Contractor: done. housebuilder = "
    << this->_housebuilder << std::endl;
}
  
Contractor::~Contractor()
{
  std::cout << "Contractor::~Contractor: done." << std::endl;
}
        
void Contractor::BuildHouse(void)
{
  _housebuilder->BuildWindow();
  _housebuilder->BuildDoor();
  _housebuilder->BuildFloor();
  std::cout << "Contractor::BuildeHouse: done." << std::endl;
}
       
House* Contractor::GetTargetHouse(void)
{
  House* house = _housebuilder->GetHouse();
  std::cout << "Contractor::GetHouse: done. house = " << house << std::endl;
  return house;
}
 
