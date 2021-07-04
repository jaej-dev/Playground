/*           
 * Title        :   03. Factory Mothod
 * Filename     :   ConcretePage.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <string>
#include "IPage.h"

/* Concrete Product class */
class IntroductionPage : public IPage
{
public:
  IntroductionPage();
  ~IntroductionPage();
  std::string GetPageName(void);
};

class BodyPage : public IPage
{
public:
  BodyPage();
  ~BodyPage();
  std::string GetPageName(void);
};

class ConclusionPage : public IPage
{
public:
  ConclusionPage();
  ~ConclusionPage();
  std::string GetPageName(void);
};

class SkillsPage : public IPage
{
public:
  SkillsPage();
  ~SkillsPage();
  std::string GetPageName(void);
};

class EducationPage : public IPage
{
public:
  EducationPage();
  ~EducationPage();
  std::string GetPageName(void);
};

class ExperiencePage : public IPage
{
public:
  ExperiencePage();
  ~ExperiencePage();
  std::string GetPageName(void);
};
