/*           
 * Title        :   09. Decorator
 * Filename     :   Decorator.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#include <iostream> 
#include <string>

#define ENABLE_DEBUG


/* Component interface */
class Computer 
{
public:
  virtual unsigned long GetCost() = 0;
  virtual std::string GetDescription() = 0;
  virtual ~Computer() {}
};

/* Conrete component */
class Desktop : public Computer
{
public:
  unsigned long GetCost()
  {
    return 1000000; // default cost
  }
  std::string GetDescription()
  {
    return "[Desktop Description]\n";
  }
};

class Labtop : public Computer
{
public:
  unsigned long GetCost()
  {
    return 2000000; // default cost
  }
  std::string GetDescription()
  {
    return "[Labtop Description]\n";
  }
};

/* Decorator */
class ComputerDecorator : public Computer
{
public:
  ComputerDecorator(Computer* c)
  {
    _computer = c;
    //std::cout << "ComputerDecorator::ComputerDecorator: done." << std::endl; 
  }
  ~ComputerDecorator()
  {
    delete _computer;
    //std::cout << "ComputerDecorator::~ComputerDecorator: done." << std::endl; 
  }

protected:
  Computer* _computer; 
};

/* Concrete decorator */
class Cpu : public ComputerDecorator 
{
public:
  Cpu(Computer *c) : ComputerDecorator(c)
  {
    //std::cout << "Cpu::Cpu:ComputerDecorator: done." << std::endl; 
  }
  ~Cpu()
  {
    //std::cout << "Cpu::~Cpu: done." << std::endl; 
  }
 
  unsigned long GetCost()
  {
    return _computer->GetCost() + GetCpuCost();
  }
 
  std::string GetDescription()
  {
    return _computer->GetDescription() + GetCpuDescription();
  }

private:
  unsigned long GetCpuCost()
  {
    return 350000;
  }
  std::string GetCpuDescription()
  {
    return " - Changed CPU: Intel SkyLake Z170\n";
  }
};

class Ram : public ComputerDecorator 
{
public:
  Ram (Computer *c) : ComputerDecorator(c) {}
 
  unsigned long GetCost()
  {
    return _computer->GetCost() + GetRamCost();
  }
 
  std::string GetDescription()
  {
    return _computer->GetDescription() + GetRamDescription();
  }

private:
  unsigned long GetRamCost()
  {
    return 70000;
  }
  std::string GetRamDescription()
  {
    return " - Changed RAM: Samsung DDR4 8G PC4-17000\n";
  }
};

