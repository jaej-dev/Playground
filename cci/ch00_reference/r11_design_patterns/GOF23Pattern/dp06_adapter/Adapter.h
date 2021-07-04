/*           
 * Title        :   06. Adapter
 * Filename     :   Adapter.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * ref: http://www.codeproject.com/Tips/595716/Adapter-Design-Pattern-in-Cplusplus
 */
#pragma once

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>

/* Abstract Target */
class AbstractPlug {
public:
  virtual ~AbstractPlug(){}
  void virtual RoundPin(){}
  void virtual PinCount(){}
};

/* Concrete Target */
class Plug : public AbstractPlug {
public:
  void RoundPin()
  {
    std::cout << "Plug::RoundPin: target called" << std::endl;
  }
  void PinCount()
  {
    std::cout << "Plug:PinCount: target called" << std::endl;
  }
};

/* Abstract Adaptee */
class AbstractSwitchBoard {
public:
  virtual ~AbstractSwitchBoard(){}
  void virtual FlatPin() {}
  void virtual PinCount() {}
};

/* Concrete Adaptee */
class SwitchBoard : public AbstractSwitchBoard
{
public:
  void FlatPin()
  {
    std::cout << "SwitchBoard::FlatPin: adaptee called." << std::endl;
  }
  void PinCount()
  {
    std::cout << "SwitchBoard::PinCount: adaptee called." << std::endl;
  }
};

/* Adapter */
class Adapter : public AbstractPlug
{
public:
  AbstractSwitchBoard *T;
  Adapter(AbstractSwitchBoard *TT)
  {
    std::cout << "Adapter::Adapter: called." << std::endl;
    T = TT;
  }
  void RoundPin()
  {
    std::cout << "Adapter::RoundPin: called." << std::endl;
    T->FlatPin();
  }
  void PinCount()
  {
    std::cout << "Adapter::PinCount: called." << std::endl;
    T->PinCount();
  }
};
