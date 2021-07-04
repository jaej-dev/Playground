/*           
 * Title        :   03. Factory Mothod
 * Filename     :   IPage.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <string>

//Create the abstract 'Product' class
class IPage
{
public:
  IPage();
  virtual ~IPage();
  virtual std::string GetPageName(void);
};
