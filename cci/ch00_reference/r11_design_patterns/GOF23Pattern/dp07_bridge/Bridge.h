/*           
 * Title        :   07. Bridge
 * Filename     :   Bridge.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#include <iostream>
#include <string>

/* Implementor */
class IDrawApi {
public:
  virtual ~IDrawApi() {}
  virtual void DrawCircle(int radius, int x, int y) {}
  virtual void DrawRectangle(int left, int top, int right, int bottom) {}
};

/* Concrete implementor*/
class RedCircle : public IDrawApi {
public:
  void DrawCircle(int radius, int x, int y)
  {
    std::cout << "RedCircle::DrawCircle: radius = " << radius << ", x = " << x
      << ", y = " << y << std::endl;
  }
};

class BlueCircle : public IDrawApi {
public:
  void DrawCircle(int radius, int x, int y)
  {
    std::cout << "BlueCircle::DrawCircle: radius = " << radius << ", x = " << x
      << ", y = " << y << std::endl;
  }
};

class BlackRectangle : public IDrawApi {
public:
  void DrawRectangle(int left, int top, int right, int bottom)
  {
    std::cout << "BlackRectangle::DrawRectangle: left = " << left 
      << ", top = " << top << ", right = " << right << ", bottom = " << bottom 
      << std::endl;
  }
};

class GrayRectangle : public IDrawApi {
public:
  void DrawRectangle(int left, int top, int right, int bottom)
  {
    std::cout << "GrayRectangle::DrawRectangle: left = " << left 
      << ", top = " << top << ", right = " << right << ", bottom = " << bottom 
      << std::endl;
  }
};

/* Abstraction */
class Shape {
public:
  Shape() {}
  Shape(IDrawApi* drawApi)
  {
    std::cout << "Shape::Shape: called." << std::endl;
    _drawApi = drawApi;
  }
  virtual ~Shape() {}
  virtual void Draw(void) {}

protected:
  IDrawApi* _drawApi;
};

/* Refined abstraction */
class Circle : public Shape {
public:
  Circle(int radius, int x, int y, IDrawApi* drawApi)
  {
    _radius = radius;
    _x = x;
    _y = y;
    _drawApi = drawApi;
  }

  void Draw(void)
  {
    std::cout << "Circle::Draw: called." << std::endl;
    _drawApi->DrawCircle(_radius, _x, _y);
  }

private:
  int _radius;
  int _x;
  int _y;
};

class Rectangle : public Shape {
public:
  Rectangle(int left, int top, int right, int bottom, IDrawApi* drawApi)
  {
    _left = left;
    _top = top;
    _right = right;
    _bottom = bottom;
    _drawApi = drawApi;
  }

  void Draw(void)
  {
    std::cout << "Rectangle::Draw: called." << std::endl;
    _drawApi->DrawRectangle(_left, _top, _right, _bottom);
  }

private:
  int _left;
  int _top;
  int _right;
  int _bottom;
};
