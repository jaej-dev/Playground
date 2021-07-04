/*           
 * Title        :   01. Abstract Factory
 * Filename     :   ConcreteFactory.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#pragma once 
#include "IRace.h"
#include "BuildingProducts.h"

class Terran : public IRace
{
public:
  Terran();
  ~Terran();
  IMaincenterBuilding* CreateMaincenterBuilding(void);
  IDepotBuilding* CreateDepotBuilding(void);
private:
  IMaincenterBuilding* _maincenter;
  IDepotBuilding* _depot;
};

class Protoss : public IRace
{
public:
  Protoss();
  ~Protoss();
  IMaincenterBuilding* CreateMaincenterBuilding(void);
  IDepotBuilding* CreateDepotBuilding(void);
private:
  IMaincenterBuilding* _maincenter;
  IDepotBuilding* _depot;
};


