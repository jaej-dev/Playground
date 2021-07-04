/*           
 * Title        :   22. Template method pattern
 * Filename     :   TemplateMethodr.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <utility>

class OperationTemplate
{
public:
  typedef std::map<std::string, std::string> Arguments;

public:
  bool Solve(const Arguments &input, Arguments &output)
  {
    assert(output.empty());
    if (!ReadInput(input))
    {
      return false;
    }
    if (!Operate()) 
    {
      return false;
    }
    WriteOutput(output);
    return true;
  }

protected:
  virtual bool ReadInput(const Arguments &input) = 0;
  virtual bool Operate() = 0;
  virtual void WriteOutput(Arguments &output) const = 0;

public:
  virtual ~OperationTemplate() {}
};

class MathOperation : public OperationTemplate
{
public:
  MathOperation(): _x(0), _y(0), _result(0), _operation(0) {}

private:
  bool ReadInput(const Arguments &input)
  {
    Arguments::const_iterator iter = input.find("x");
    if (input.end() == iter)
    {
      return false;
    }
    std::istringstream in(iter->second);
    in >> _x;
    if (in.fail())
    {
      return false;
    }
    iter = input.find("y");
    if (input.end() == iter)
    {
      return false;
    }
    in.clear();
    in.str(iter->second);
    in >> _y;
    if (in.fail())
    {
        return false;
    }
    iter = input.find("operation");
    if (input.end() == iter || iter->second.size() != 1)
    {
      return false;
    }
    _operation = iter->second[0];

    return true;
  }

  bool Operate()
  {
    switch (_operation)
    {
      case '+':
        _result = _x + _y;
        break;
      case '-':
        _result = _x - _y;
        break;
      case '*':
        _result = _x * _y;
        break;
      case '/':
        if (0 == _y)
        {
            return false;
        }
        _result = _x / _y;
        break;
      default:
        return false;
    }
    return true;
  }

  void WriteOutput(Arguments &output) const
  {
    std::ostringstream out;
    out << _result;
    output.insert(std::make_pair(std::string("result"), out.str()));
  }

private:
  int _x, _y, _result;
  char _operation;
};

class ListOperation : public OperationTemplate
{
private:
  bool ReadInput(const Arguments &input)
  {
    _list.clear();

    Arguments::const_iterator i = input.find("array");
    if (input.end() == i)
    {
      return false;
    }
    std::istringstream in(i->second);
    typedef std::istream_iterator<int> T;
    std::copy(T(in), T(), std::back_inserter(_list));
    if (!in.eof())
    {
      return false;
    }
    return true;
  }

  bool Operate()
  {
    _list.reverse();
    return true;
  }

  void WriteOutput(Arguments &output) const
  {
    std::ostringstream out;
    std::copy(_list.begin(), _list.end(), std::ostream_iterator<int>(out, " "));
    output.insert(std::make_pair(std::string("result"), out.str()));
  }

private:
  std::list<int> _list;
};
