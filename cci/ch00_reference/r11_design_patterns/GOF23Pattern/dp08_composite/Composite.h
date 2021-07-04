/*           
 * Title        :   08. Composite
 * Filename     :   Composite.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#include <vector>
#include <iostream> 
#include <algorithm> 
#include <functional> 
using namespace std;

/* Component */
class Graphic {
public:
  virtual void Draw() const = 0;
  virtual void Remove(Graphic *g) {}
  virtual void Add(Graphic *g) {}
  virtual void GetChild(int) {}
  virtual ~Graphic() {}
};
 
class Line : public Graphic {
public:
  void Draw() const
  {
    std::cout << "Line::Draw: done." << std::endl;
  }
};

class Circle : public Graphic {
public:
  void Draw() const
  {
    std::cout << "Circle::Draw: done." << std::endl;
  }
};

class Rectangle : public Graphic {
public:
  void Draw() const
  {
    std::cout << "Rectagle::Rectangle: done." << std::endl;
  }
};


/* Composite */
class Picture : public Graphic {
public:
  void Draw() const
  {
    std::cout << "Picture::Draw: _glist size = " << _glist.size()
      << std::endl;
    // for each element in _glist, call the draw member function
    for_each(_glist.begin(), _glist.end(), mem_fun(&Graphic::Draw));
  }
 
  void Add(Graphic* g)
  {
    _glist.push_back(g);
    std::cout << "Picture::Add: _glist size = " << _glist.size()
      << std::endl;
  }

  void Remove(Graphic* g)
  {
    std::vector<Graphic* >::iterator it;
    it = std::find(_glist.begin(), _glist.end(), g);
    _glist.erase(it);
    std::cout << "Picture::Remove: _glist size = " << _glist.size()
      << std::endl;
  }
 
private:
  vector<Graphic*> _glist;
};
