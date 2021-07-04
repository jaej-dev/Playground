/*           
 * Title        :   16. Iterator pattern
 * Filename     :   Iterator.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>
#include <vector>
#include <algorithm>  /* std::find */

/* Context */
class Drink
{
public:
  Drink(std::string name)
  {
    _name = name;
    std::cout << "Drink::Drink: created " << _name << std::endl;
  }

  std::string GetName()
  {
    return _name;
  }

private:
  std::string _name;
};

/* Iterator */
class IIterator
{
public:
  virtual ~IIterator() {}
  virtual bool IsDone() = 0;
  virtual void* Next() = 0;
};

/* Aggregate */
class IAggregate
{
public:
  virtual IIterator* CreateIterator() = 0;
  virtual ~IAggregate() {}
};

/* Concrete Iterator */
class VendingMachineIterator : public IIterator
{
public:
  VendingMachineIterator(std::vector<Drink*> d)
  {
    _pos = 0;
    for (size_t i = 0; i < d.size(); ++i)
    {     
      _drinkList.push_back(d[i]);
    }
    std::cout << "VendringMachineIterator::VendingMachineIterator: done"
      << std::endl;
  }

  virtual ~VendingMachineIterator()
  {
    std::cout << "VendringMachineIterator::~vendingMachineIterator: done"
      << std::endl;
  }

  bool IsDone()
  {
    if( _pos < _drinkList.size())
      return true;

    return false;
  }

  void* Next()
  {
    Drink* d = _drinkList[_pos];
    _pos++;
    return d;
  }

private:
  std::vector<Drink*> _drinkList;
  size_t _pos;   
};

/* Concrete Aggregate */
class VendingMachineAggregate : public IAggregate
{
public:
  VendingMachineAggregate()
  {
    std::cout << "VendingMachingAggregate::VendingMachineAggregate: done" 
      << std::endl;
  }

  ~VendingMachineAggregate()
  {
    ClearDrink();
    delete _vm;
    std::cout << "VendingMachingAggregate::~VendingMachineAggregate: done" 
      << std::endl;
  }

  void AddDrink(std::string name)
  {
    Drink* d = new Drink(name);
    _drinkList.push_back(d);
  }

  void RemoveDrink(std::string name)
  {
    std::vector<Drink*>::iterator it = _drinkList.begin();

    for (size_t i = 0; i < _drinkList.size(); ++i, ++it)
    {
      if ( _drinkList[i]->GetName() == name)
      {
        std::cout << "VendingMachineAggregate::RemoveDrink: removed " 
          << _drinkList[i]->GetName() << std::endl;
        _drinkList.erase(it);
        delete _drinkList[i];
        break;
      }
    }
  }

  void ClearDrink(void)
  {
    // force delete dynamic vectar of object pointer
    for (size_t i = 0; i < _drinkList.size(); ++i)
    {
      delete _drinkList[i];
    }
    _drinkList.clear();
  }

  IIterator* CreateIterator()
  { 
    _vm = new VendingMachineIterator(_drinkList); 
    return _vm;
  }

private:
  std::vector<Drink*> _drinkList;
  VendingMachineIterator* _vm;
};

