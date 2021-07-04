/*           
 * Title        :   04. Prototype
 * Filename     :   ConcreteColourPrototype.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#include <iostream>
#include <iomanip>
#include "ConcreteColourPrototype.h"

#ifdef __cplusplus
#endif

Colour::Colour(int red, int green, int blue)
{
  _red = red, _green = green, _blue = blue;
}
  
ColourPrototype* Colour::Clone(void)
{
  std::cout << "Colour::Clone: cloning colour RGB = " 
    << std::setw(3) << _red << ", " 
    << std::setw(3) << _green << ", " 
    << std::setw(3) << _blue << std::endl;

  ColourPrototype* colourPrototype = new Colour(_red, _green, _blue);
  
  return colourPrototype;
}
  
void Colour::SetRed(int red)
{
  _red = red;
}
  
void Colour::SetGreen(int green)
{
  _green = green;
}
  
void Colour::SetBlue(int blue)
{
  _blue = blue;
}
  
int Colour::GetRed(void)
{
  return _red;
}
  
int Colour::GetGreen(void)
{
  return _green;
}
  
int Colour::GetBlue(void)
{
  return _blue;
}

