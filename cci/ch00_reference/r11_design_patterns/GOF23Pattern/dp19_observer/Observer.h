/*           
 * Title        :   19. Observer pattern
 * Filename     :   Observer.h
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

/* Observer */
class IObserver
{
public:
  IObserver()
  {
    std::cout << "IObserver::IObserver: done." << std::endl;
  }
  virtual ~IObserver()
  {
    std::cout << "IObserver::~IObserver: done." << std::endl;
  }
  virtual void Update(int msg) = 0;
};

/* Subject */
class ISubject
{
public:
  ISubject()
  {
    std::cout << "ISubject::ISubject: done." << std::endl;
  }
  
  virtual ~ISubject()
  {
    std::cout << "ISubject::ISubject: done." << std::endl;
  }
  
  virtual void Subscribe(IObserver* o)
  {
    _observers.push_back(o);
  }

  virtual void Unsubscribe (IObserver * o)
  {
    int count = _observers.size(); 
    int i; 
    for (i = 0; i < count; ++i)
    { 
      if(_observers[i] == o) 
        break; 
    } 
    if(i < count) 
      _observers.erase(_observers.begin() + i);
  }

  virtual void Notify(int msg)
  {
    int count = _observers.size(); 
    for (int i = 0; i < count; ++i) 
      (_observers[i])->Update(msg); 
  }

private:
  std::vector<IObserver *> _observers;
};

/* Concrete subject */
class MySubject : public ISubject
{
public:
  enum Message { ADD, REMOVE };
  MySubject()
  {
    std::cout << "MySubject::MySubject: done." << std::endl;
  }
  ~MySubject()
  {
    std::cout << "MySubject::~MySubject: done." << std::endl;
  }
};

/* Concrete observer */
class MyObserver : public IObserver
{
public:
  explicit MyObserver(const std::string &str) : _name(str) 
  {
    std::cout << "MyObserver::MyObserver: " << _name << std::endl;
  }
  ~MyObserver()
  {
    std::cout << "MyObserver::~MyObserver: " << _name << std::endl;
  }
  void Update(int msg)
  {
    std::cout << "MyObserver::Update: " << _name << " Got message " << msg
      << std::endl;
  }

private:
  std::string _name;
};
