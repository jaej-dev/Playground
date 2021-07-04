/*           
 * Title        :   04. Prototype
 * Filename     :   ColourPrototype.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif


class ColourPrototype
{
public:
  ColourPrototype();
  virtual ~ColourPrototype();
  virtual ColourPrototype* Clone(void);
};
