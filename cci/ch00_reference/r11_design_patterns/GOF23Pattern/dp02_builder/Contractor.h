/*           
 * Title        :   02. Builder
 * Filename     :   Contractor.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include "ConcreteHousePlan.h"
#include "ConcreteHouseBuilder.h"
 
/* The Director. Constructs the house */
class Contractor
{
public:
  Contractor();
  Contractor(IHouseBuilder* housebuilder);
  ~Contractor();
        
  void BuildHouse(void);
  House * GetTargetHouse(void);

private:
  IHouseBuilder* _housebuilder;
};
 
