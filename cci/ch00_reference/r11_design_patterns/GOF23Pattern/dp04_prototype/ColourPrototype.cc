/*           
 * Title        :   04. Prototype
 * Filename     :   ColourPrototype.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "ColourPrototype.h"

ColourPrototype::ColourPrototype()
{
  //std::cout << "ColourPrototype::ColourPrototype: done." << std::endl;
}

ColourPrototype::~ColourPrototype()
{
  //std::cout << "ColourPrototype::~ColourPrototype: done." << std::endl;
}

ColourPrototype* ColourPrototype::Clone(void)
{
  std::cout << "ColourPrototype::Clone: done." << std::endl;
  return NULL;
}
