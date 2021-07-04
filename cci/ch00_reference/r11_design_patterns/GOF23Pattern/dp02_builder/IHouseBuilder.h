/*           
 * Title        :   02. Builder
 * Filename     :   IHouseBuilder.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <string>
#include "ConcreteHousePlan.h"

/* Builder class */
class IHouseBuilder
{
public :
  IHouseBuilder();
  /* Abstract functions to build parts */
  virtual ~IHouseBuilder();
  virtual void BuildWindow(void);
  virtual void BuildDoor(void);
  virtual void BuildFloor(void);
  /* The product is returned by this function */
  virtual House* GetHouse(void);
};
