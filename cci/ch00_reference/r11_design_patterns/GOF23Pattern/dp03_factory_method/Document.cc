/*           
 * Title        :   03. Factory Mothod
 * Filename     :   Document.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "Document.h"

Document::Document()
{
  /* Should be called in the derived class */
  std::cout << "Document::Document: done." << std::endl;
}

Document::~Document()
{
  /* Should be called in the derived class */
  for(std::list<IPage*>::iterator it = _pages.begin(); 
      it != _pages.end(); it++)
  {
    IPage * temp = (IPage *)*it;
    //list<IPage*>& doc1Pages = const_cast<list<IPage*>&> (doc1->GetPages());
    std::cout << "Doucument::~Document: deleted item = " << (*it)->GetPageName() 
      << std::endl;
    delete temp;
  }
  
  std::cout << "Document::~Document: done." << std::endl;
}

void Document::AddPage(IPage* page)
{
  _pages.push_back(page);
  std::cout << "Document::AddPage: done." << std::endl;
}

const std::list<IPage*>& Document::GetPages(void)
{
  std::cout << "Document::GetPages: done." << std::endl;
  return _pages;
}

void Document::CreatePages(void)
{
  /* Factory method should be called in the derived class */
  std::cout << "Document::CreatePages: done." << std::endl;
}

