/*           
 * Title        :   04. Prototype
 * Filename     :   ColourManager.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once
#include <string>
#include <map>
#include "ConcreteColourPrototype.h"

#ifdef __cplusplus
#endif

/* Prototype manager */
class ColourManager
{
  public:
  ColourManager();
  virtual ~ColourManager();
  void AddColour(const std::string& colour, ColourPrototype* prototype);
  ColourPrototype* GetColour(const std::string& colour);
  void PrintColours(void);

private:
  std::map<std::string, ColourPrototype*> _coloursMap;
};
