/*           
 * Title        :   03. Factory Mothod
 * Filename     :   ConcretePage.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#ifdef __cplusplus
#endif

#include "ConcretePage.h"

/* Concrete Product class*/
IntroductionPage::IntroductionPage()
{
}

IntroductionPage::~IntroductionPage()
{
}

std::string IntroductionPage::GetPageName(void)
{
  return "IntroductionPage";
}

BodyPage::BodyPage()
{
}

BodyPage::~BodyPage()
{
}

std::string BodyPage::GetPageName(void)
{
  return "BodyPage";
}

ConclusionPage::ConclusionPage()
{
}

ConclusionPage::~ConclusionPage()
{
}

std::string ConclusionPage::GetPageName(void)
{
  return "ConclusionPage";
}

SkillsPage::SkillsPage()
{
}

SkillsPage::~SkillsPage()
{
}

std::string SkillsPage::GetPageName(void)
{
  return "SkillsPage";
}

EducationPage::EducationPage()
{
}

EducationPage::~EducationPage()
{
}

std::string EducationPage::GetPageName(void)
{
  return "EducationPage";
}

ExperiencePage::ExperiencePage()
{
}

ExperiencePage::~ExperiencePage()
{
}

std::string ExperiencePage::GetPageName(void)
{
  return "ExperiencePage";
}
