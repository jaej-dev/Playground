/*           
 * Title        :   02. Builder
 * Filename     :   ConcreteHouseBuilder.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include "IHouseBuilder.h"

/* Concrete class for the builder interface */
class LavishHouse : public IHouseBuilder
{
public:
  LavishHouse();
  ~LavishHouse();
       
  void BuildWindow(void);
  void BuildDoor(void);
  void BuildFloor(void);
  House* GetHouse(void);

private:
  House* _house;
};
 
/* Another Concrete class for the builder interface */
class NormalHouse : public IHouseBuilder
{
public:
  NormalHouse();
  ~NormalHouse();
       
  void BuildWindow(void);
  void BuildDoor(void);
  void BuildFloor(void);
  House* GetHouse(void);

private:
  House* _house;
};
 
