/*           
 * Title        :   04. Prototype
 * Filename     :   ConcreteColourPrototype.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once
#include "ColourPrototype.h"

#ifdef __cplusplus
#endif

/* The 'ConcretePrototype' class */
class Colour : public ColourPrototype
{
public:
  Colour(int red, int green, int blue);
  ColourPrototype* Clone(void);
  void SetRed(int red);
  void SetGreen(int green);
  void SetBlue(int blue);
  int GetRed(void);
  int GetGreen(void);
  int GetBlue(void);

private:
  Colour(); /* default constructor not allowed */
  int _red, _green, _blue;
};
