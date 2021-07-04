/*           
 * Title        :   17. Mediator pattern
 * Filename     :   Mediator.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */

#include "Mediator.h"


Colleage::Colleage(Mediator* m)
{
  _mediator = m;
  std::cout << "Colleage::Colleage: done. " << this << std::endl;
}

Colleage::~Colleage()
{
  std::cout << "Colleage::~Colleage: done. " << this << std::endl;
}

void Colleage::SetMediator(Mediator* m)
{
  _mediator = m;
}

void ConcreteColleageA::SendMsg(std::string msg)
{
  _mediator->SendMsg(msg, this);
}

void ConcreteColleageA::GetMsg(std::string msg)
{
  std::cout << "ConcreteColleageA::GetMsg: " << msg << std::endl;
}

void ConcreteColleageN:: SendMsg(std::string msg)
{
  _mediator->SendMsg(msg, this);
}

void ConcreteColleageN::GetMsg(std::string msg)
{
  std::cout << "ConcreteColleageN::GetMsg: " << msg << std::endl;
}

Mediator::Mediator()
{
  std::cout << "Mediator::Mediator: done. " << this << std::endl;
}

Mediator::~Mediator()
{
  std::cout << "Mediator::~Mediator: done." << this << std::endl;
}

void ConcreteMediator::SetColleageA(Colleage* c)
{
  this->_colleageA = c;
  std::cout << "ConcreteMediator::SetColleageA: " << c << std::endl;
}

void ConcreteMediator::SetColleageN(Colleage* c)
{
  this->_colleageN = c;
  std::cout << "ConcreteMediator::SetColleageN: " << c << std::endl;
}

void ConcreteMediator::SendMsg(std::string msg, Colleage* c)
{
  if (c == this->_colleageA)
  {
    this->_colleageN->GetMsg(msg);
  }
  else if (c == this->_colleageN)
  {
    this->_colleageA->GetMsg(msg);
  }
}
