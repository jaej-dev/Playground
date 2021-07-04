/*           
 * Title        :   17. Mediator pattern
 * Filename     :   Mediator.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>

class Mediator;

/* Colleage */ 
class Colleage
{
public:
  virtual ~Colleage();
  virtual void SetMediator(Mediator* m);
  virtual void SendMsg(std::string) = 0;
  virtual void GetMsg(std::string) = 0;

protected:
  Colleage(Mediator* m);
  Mediator* _mediator;
};

/* Concrete colleage */ 
class ConcreteColleageA : public Colleage
{
public:
  ConcreteColleageA(Mediator* m) : Colleage(m) {}
  ~ConcreteColleageA() {}
  virtual void SendMsg(std::string msg);
  virtual void GetMsg(std::string msg);
};

class ConcreteColleageN : public Colleage
{
public:
  ConcreteColleageN(Mediator* m) : Colleage(m) {}
  ~ConcreteColleageN() {}
  virtual void SendMsg(std::string msg);
  virtual void GetMsg(std::string msg);
};

/* Mediator */
class Mediator
{
public:
  virtual ~Mediator();
  virtual void SendMsg(std::string, Colleage*) = 0;

protected:
  Mediator();
};

/* Concrete Mediator */ 
class ConcreteMediator : public Mediator
{
public:
  ConcreteMediator() {}
  ~ConcreteMediator() {}
  void SetColleageA(Colleage* c);
  void SetColleageN(Colleage* c);
  virtual void SendMsg(std::string msg, Colleage* c);

private:
  Colleage* _colleageA;
  Colleage* _colleageN;
};
