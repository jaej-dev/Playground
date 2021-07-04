/*           
 * Title        :   01. Abstract Factory
 * Filename     :   IRace.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#pragma once 
#include <string>
#include "IMaincenterBuilding.h"
#include "IDepotBuilding.h"

class IRace
{
public:
  IRace();
 /* NOTE: must be virtual destroyer for child class destroy procedure */
  virtual ~IRace();
  virtual IMaincenterBuilding * CreateMaincenterBuilding(void);
  virtual IDepotBuilding * CreateDepotBuilding(void);
};

