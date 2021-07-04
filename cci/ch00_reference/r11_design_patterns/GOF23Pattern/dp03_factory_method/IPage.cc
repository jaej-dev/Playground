/*           
 * Title        :   03. Factory Mothod
 * Filename     :   IPage.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <string>
#include "IPage.h"

IPage::IPage()
{
}
IPage::~IPage()
{
}
std::string IPage::GetPageName(void)
{
  return "";
}
