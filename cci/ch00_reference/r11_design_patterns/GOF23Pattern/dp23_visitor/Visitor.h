/*           
 * Title        :   23. Visitor pattern
 * Filename     :   Visitor.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>
#include <list>

// Forward Declaration
class CarPartVisitor;
class Body;
class Engine;
class Car;

/* Element interface */
class CarPart
{
public:
  virtual void Accept(CarPartVisitor* v) = 0;
  virtual bool GetState(void) = 0;
  virtual std::string GetPartName(void) = 0;
  virtual ~CarPart() {}
};

/* Visitor interface  */
class CarPartVisitor
{
public:
  virtual void Visit(Body* b) = 0;
  virtual void Visit(Engine* e) = 0;
  virtual void Visit(Car* c) = 0;
  virtual ~CarPartVisitor() {}
};

/* Concrete element */
class Body : public CarPart
{
public:
  Body(std::string name, bool state) : _partName(name), _partState(state) {}
  void Accept(CarPartVisitor* v)
  {
    v->Visit(this);
    std::cout << "Body::Accept: done." << std::endl;
  }
  bool GetState(void)
  {
    return CheckPartState();
  }
  bool CheckPartState(void)
  {
    std::cout << "Body::CheckPartState: " << _partName << " : " << _partState
      << std::endl;
    return _partState;
  }
  std::string GetPartName(void)
  {
    return _partName;
  }

public:  
  ~Body() {}

private:
  std::string _partName;
  bool _partState;
};

class Engine : public CarPart
{
public:  
  Engine(std::string name, bool state) : _partName(name), _partState(state) {}
  void Accept(CarPartVisitor* v)
  {
    v->Visit(this);
    std::cout << "Engine::Accept: done." << std::endl;
  }
  bool GetState(void)
  {
    return CheckPartState();
  }
  bool CheckPartState(void)
  {
    std::cout << "Engine::CheckPartState: " << _partName << " : " << _partState
      << std::endl;
    return _partState;
  }
  std::string GetPartName(void)
  {
    return _partName;
  }

public:  
  ~Engine() {}

private:
  std::string _partName;
  bool _partState;
};

class Car : public CarPart
{
public:  
  Car(std::string name) : _partName(name)
  {
    std::cout << "Car::Car: created " << _partName << std::endl;
  }
  void Accept(CarPartVisitor* v)
  {
    for (std::list<CarPart*>::iterator it = _partList.begin(); 
      it != _partList.end(); it++) 
    {
      (*it)->Accept(v);
    }
    v->Visit(this);
    GetState(); // check all parts
    std::cout << "Car::Accept: done." << std::endl;
  }
  bool GetState(void)
  {
    int defectCount = 0;
    for (std::list<CarPart*>::iterator it = _partList.begin(); 
      it != _partList.end(); it++) 
    {
      if (!(*it)->GetState())
      {
        ++defectCount;
      }
    }
    std::cout << "Car::GetState: found " << defectCount << " defects" 
      << std::endl;
    return (defectCount == 0)?true:false;
  }
  std::string GetPartName(void)
  {
    return _partName;
  }
  void AddPart(CarPart* c)
  {
    std::cout << "Car::AddPart: added " << c->GetPartName() << std::endl;
    _partList.push_back(c);
  }

public:
  ~Car() {}

private:
  std::string _partName;
  std::list<CarPart*> _partList;
};

/* Concrete visitor */
class CarPartInspector : public CarPartVisitor
{
public:
  CarPartInspector()
  {
    std::cout << "CarPartInspector:CarPartInspector: done." << std::endl;
  }
  void Visit(Body* b)
  {
    std::cout << "CarPartInspector:Visit: visit to body..." << std::endl;
  }
  void Visit(Engine* e)
  {
    std::cout << "CarPartInspector:Visit: visit to engine..." << std::endl;
  }
  void Visit(Car* c)
  {
    std::cout << "CarPartInspector:Visit: visit to car..." << std::endl;
  }
  ~CarPartInspector()
  {
    std::cout << "CarPartInspector:~CarPartInspector: done." << std::endl;
  }
};

