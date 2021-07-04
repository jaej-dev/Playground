/*           
 * Title        :   02. Builder
 * Filename     :   IHousePlan.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <string>
 
/* Interface that will be returned as the product from builder */
class IHousePlan
{
public:
  IHousePlan();
  virtual ~IHousePlan();
  virtual void SetWindow(std::string window);
  virtual void SetDoor(std::string door);
  virtual void SetFloor(std::string floor);
};
 
