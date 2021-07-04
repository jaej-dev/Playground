/*           
 * Title        :   02. Builder
 * Filename     :   ConcreteHousePlan.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <string>
#include "IHousePlan.h"
 
/* Concrete class for the HousePlan interface */
class House : public IHousePlan
{
public :
  House();
  ~House();
  void SetWindow(std::string window);
  void SetDoor(std::string door);
  void SetFloor(std::string floor);

private :
  std::string _window;
  std::string _door;
  std::string _floor;
};
 
