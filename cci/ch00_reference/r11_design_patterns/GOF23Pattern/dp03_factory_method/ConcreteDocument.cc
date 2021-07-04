/*           
 * Title        :   03. Factory Mothod
 * Filename     :   ConcreteDocument.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include "ConcreteDocument.h"
#include "ConcretePage.h"

/* Create the 'ConcreteCreator' class */
Report::Report()
{
  CreatePages();
  std::cout << "Report::Report: done." << std::endl;
}

//Report::~Report()
//{
//  std::cout << "Report::~Report: done." << std::endl;
//}

void Report::CreatePages(void)
{
  AddPage(new IntroductionPage());
  AddPage(new BodyPage());
  AddPage(new ConclusionPage());
  std::cout << "Report::CreatePages: done." << std::endl;
}

Resume::Resume()
{
  CreatePages();
  std::cout << "Resume::Resume: done." << std::endl;
}

//Resume::~Resume()
//{
//  std::cout << "Resume::~Resume: done." << std::endl;
//}

void Resume::CreatePages(void)
{
  AddPage(new SkillsPage());
  AddPage(new EducationPage());
  AddPage(new ExperiencePage());
  std::cout << "Resume::CreatePages: done." << std::endl;
}

