/*           
 * Title        :   18. Memento pattern
 * Filename     :   Memento.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>
#include <stack>

/* Memento */
class Memento
{
public:
  Memento(std::string s)
  {
    _state = s;
  }
  std::string GetState() const
  {
    return _state;
  }

private:
  std::string _state;
};

/* Originator */
class Originator
{
public:
  Originator()
  {
    std::cout << "Originator::Originator: done." << std::endl;
  }

  ~Originator()
  {
    std::cout << "Originator::~Originator: done." << std::endl;
  }

  void SetState(std::string s)
  {
    _state = s;
    std::cout << "Originator::SetState: " << _state << std::endl;
  }

  void SetMemento(Memento* m)
  {
    if (m)
    {
      _state = m->GetState();
      std::cout << "Originator::SetMemento: " << _state << std::endl;
      delete m;
    }
  }

  Memento* CreateMemento(void)
  {
    std::cout << "Originator::CreateMemento: " << _state << std::endl;
    return new Memento(_state);
  }

private:
  std::string _state;
};

/* Caretaker */
class Caretaker
{
public:
  Caretaker()
  {
    std::cout << "Caretaker::Caretaker: done." << std::endl;
  }
  
  ~Caretaker()
  {
    while (!_stack.empty())
    {
      Memento* m = _stack.top();
      delete m;
      _stack.pop();
    }
    std::cout << "Caretaker::~Caretaker: done." << std::endl;
  }

  void AddMemento(Memento* m)
  {
    _stack.push(m);
  }

  Memento* UndoMemento(void)
  {
    if (_stack.empty())
    {
      std::cout << "Caretaker::UndoMemento: empty." << std::endl;
      return NULL;
    }

    Memento* m = _stack.top();
    _stack.pop();
    return m;
  }

private:
  std::stack<Memento*> _stack;
};
