/*           
 * Title        :   03. Factory Mothod
 * Filename     :   Document.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <string>
#include <list>
#include "IPage.h"

/* Create the abstract 'Creator' class */
class Document
{
public:
  Document();
  virtual ~Document();
  void AddPage(IPage* page);
  const std::list<IPage*>& GetPages(void);

  /* Factory Method */
  virtual void CreatePages(void);
  //virtual void DeletePages(void);

private:
  std::list<IPage*> _pages;
};
