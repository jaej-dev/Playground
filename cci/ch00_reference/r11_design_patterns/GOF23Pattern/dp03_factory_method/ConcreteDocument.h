/*           
 * Title        :   03. Factory Mothod
 * Filename     :   ConcreteDocument.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include "Document.h"

/* Create the 'ConcreteCreator' class */
class Report : public Document
{
public:
  Report();
//  ~Report();
  void CreatePages(void);
};

class Resume : public Document
{
public:
  Resume();
//  ~Resume();
  void CreatePages(void);
};

