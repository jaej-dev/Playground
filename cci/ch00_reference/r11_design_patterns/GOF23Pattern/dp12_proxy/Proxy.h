/*           
 * Title        :   12. Proxy Pattern
 * Filename     :   Proxy.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>

/* Subject interface */
class IMath
{
public:
  virtual ~IMath() {}
  virtual double Add(double x, double y) = 0;
  virtual double Sub(double x, double y) = 0;
  virtual double Mul(double x, double y) = 0;
  virtual double Div(double x, double y) = 0;
};

/* RealSubjects */
class Math : public IMath
{
public:
  Math()
  {
    std::cout << "Math::Math: done." << std::endl;
  }
  ~Math()
  {
    std::cout << "Math::~Math: done." << std::endl;
  }
  double Add(double x, double y)
  {
    std::cout << "Math:Add: x = " << x << ", y = " << y << std::endl;
    return x + y;
  }
  double Sub(double x, double y)
  {
    return x - y;
  }
  double Mul(double x, double y)
  {
    return x * y;
  }
  double Div(double x, double y)
  {
    return x / y;
  }
};

/* Proxy */
class MathProxy : public IMath
{
public:
  MathProxy()
  {
    _math = NULL;
    std::cout << "MathProxy:MathProxy: done." << std::endl;
  }
  virtual ~MathProxy()
  {
    if(_math)
      delete _math;
    std::cout << "MathProxy:~MathProxy: done." << std::endl;
  }
  double Add(double x, double y)
  {
    std::cout << "MathProxy:Add: x = " << x << ", y = " << y << std::endl;
    return GetMathInstance()->Add(x, y);
  }
  double Sub(double x, double y)
  {
    return GetMathInstance()->Sub(x, y);
  }
  double Mul(double x, double y)
  {
    return GetMathInstance()->Mul(x, y);
  }
  double Div(double x, double y)
  {
    return GetMathInstance()->Div(x, y);
  }

private:
  Math* _math;
  Math* GetMathInstance(void)
  {
    if(!_math)
      _math = new Math();
    return _math;
  }
};
