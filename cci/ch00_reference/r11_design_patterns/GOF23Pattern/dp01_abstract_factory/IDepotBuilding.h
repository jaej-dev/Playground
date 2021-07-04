/*           
 * Title        :   01. Abstract Factory
 * Filename     :   IDepotBuilding.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#pragma once 
#include <string>
#include "IMaincenterBuilding.h"

class IDepotBuilding
{
public:
  IDepotBuilding();
  virtual ~IDepotBuilding();

  virtual void Interact(IMaincenterBuilding b);
};

