/*           
 * Title        :   01. Abstract Factory
 * Filename     :   BuildingProducts.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */

#pragma once 
#include <string>
#include "IDepotBuilding.h"

class CommandCenter : public IMaincenterBuilding
{
public:
  CommandCenter();
  ~CommandCenter();
};

class SupplyDepot : public IDepotBuilding
{
public:
  SupplyDepot();
  ~SupplyDepot();
  void Interact(IMaincenterBuilding b);
};

class Nexus : public IMaincenterBuilding
{
public:
  Nexus();
  ~Nexus();
};


class Pylon : public IDepotBuilding
{
public:
  Pylon();
  ~Pylon();
  void Interact(IMaincenterBuilding b);
};
