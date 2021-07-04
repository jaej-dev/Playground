/*           
 * Title        :   04. Prototype
 * Filename     :   ColourManager.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#include <iostream>
#include <iomanip>
#include "ColourManager.h"

#ifdef __cplusplus
#endif

ColourManager::ColourManager()
{
  std::cout << "ColourManager::ColourManager: done." << std::endl;
}

ColourManager::~ColourManager()
{
  while(!_coloursMap.empty())
  {
    std::map<std::string, ColourPrototype*>::iterator it = _coloursMap.begin();
    delete it->second;
    _coloursMap.erase(it);
  }
  std::cout << "ColourManager::~ColourManager: done." << std::endl;
}
  
void ColourManager::AddColour(const std::string& colour, 
    ColourPrototype* prototype)
{
  _coloursMap[colour] = prototype;
  std::cout << "ColourManager::AddColour: " << colour << " (";
  std::cout << std::setw(3) << dynamic_cast<Colour*>(prototype)->GetRed() 
    << ", ";
  std::cout << std::setw(3) << dynamic_cast<Colour*>(prototype)->GetGreen() 
    << ", ";
  std::cout << std::setw(3) << dynamic_cast<Colour*>(prototype)->GetBlue() 
    << ")"<< std::endl;
}
  
ColourPrototype* ColourManager::GetColour(const std::string& colour)
{
  std::map<std::string, ColourPrototype*>::const_iterator it = \
    _coloursMap.find(colour);
  if(it != _coloursMap.end())
  {
    std::cout << "ColourManager::GetColour: " << colour << " <";
    std::cout << std::setw(3) << dynamic_cast<Colour*>(it->second)->GetRed() 
      << ", ";
    std::cout << std::setw(3) << dynamic_cast<Colour*>(it->second)->GetGreen() 
      << ", ";
    std::cout << std::setw(3) << dynamic_cast<Colour*>(it->second)->GetBlue() 
      << ">"<< std::endl;
    return it->second;
  }
  return NULL;
}

void ColourManager::PrintColours(void)
{
  std::cout << "ColourManager::PrintColours:" << std::endl;
  std::map<std::string, ColourPrototype*>::const_iterator it = \
    _coloursMap.begin();
  while(it != _coloursMap.end())
  {
    std::cout << std::setw(15) << it->first << " : ";
    std::cout << std::setw(3)  << dynamic_cast<Colour*>(it->second)->GetRed() 
      << ", ";
    std::cout << std::setw(3)  << dynamic_cast<Colour*>(it->second)->GetGreen() 
      << ", ";
    std::cout << std::setw(3)  << dynamic_cast<Colour*>(it->second)->GetBlue() 
      << std::endl;
    ++it;
  }
}

