/*           
 * Title        :   15. Interpreter pattern
 * Filename     :   Interpreter.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * This source code have two problems as below.
 * 1) Source and destination overlap in strcpy 
 * 2) Conditional jump or move depends on uninitialised values
 */
#pragma once

#include <iostream> 
#include <string>
#include <cstring>

/* AbstracExpression */
class RNInterpreter
{
public:
  RNInterpreter();
  RNInterpreter(int) {}
  virtual ~RNInterpreter();
  
  int interpret(char* input);

  virtual void interpret(char *input, int &total)
  {
    // for internal use
    int index;
    index = 0;
    if (!std::strncmp(input, nine(), 2))
    {
      total += 9 * multiplier();
      index += 2;
    }
    else if (!std::strncmp(input, four(), 2))
    {
      total += 4 * multiplier();
      index += 2;
    }
    else
    {
      if (input[0] == five())
      {
        total += 5 * multiplier();
        index = 1;
      }
      else
      {
        index = 0;
      }
      for (int end = index + 3; index < end; index++)
      {
        if (input[index] == one())
        {
          total += 1 * multiplier();
        }
        else
        {
          break;
        }
      }
    }
    std::strcpy(input, &(input[index]));
  } // remove leading chars processed

protected:
  // cannot be pure virtual because client asks for instance
  virtual char one() { return '\0'; }
  virtual char* four() { return NULL; }
  virtual char five() { return '\0'; }
  virtual char* nine() { return NULL; }
  virtual int multiplier() { return 0; }

private:
  RNInterpreter* _thousands;
  RNInterpreter* _hundreds;
  RNInterpreter* _tens;
  RNInterpreter* _ones;
};

class Thousand : public RNInterpreter
{
public:
  // provide 1-arg ctor to avoid infinite loop in base class ctor
  Thousand(int) : RNInterpreter(1) {}
  ~Thousand() {}

protected:
  char one()
  {
    return 'M';
  }
  char *four()
  {
    return (char*) "";
  }
  char five()
  {
    return '\0';
  }
  char* nine()
  {
    return (char*) "";
  }
  int multiplier()
  {
    return 1000;
  }
};

class Hundred : public RNInterpreter
{
public:
  Hundred(int) : RNInterpreter(1) {}
  ~Hundred() {}

protected:
  char one()
  {
    return 'C';
  }
  char* four()
  {
    return (char*) "CD";
  }
  char five()
  {
    return 'D';
  }
  char* nine()
  {
    return (char*) "CM";
  }
  int multiplier()
  {
    return 100;
  }
};

class Ten : public RNInterpreter
{
public:
  Ten(int) : RNInterpreter(1) {}
  ~Ten() {}

protected:
  char one()
  {
    return 'X';
  }
  char* four()
  {
    return (char*) "XL";
  }
  char five()
  {
    return 'L';
  }
  char *nine()
  {
    return (char*) "XC";
  }
  int multiplier()
  {
    return 10;
  }
};

class One : public RNInterpreter
{
public:
  One(int) : RNInterpreter(1) {}
  ~One() {}

protected:
  char one()
  {
    return 'I';
  }
  char* four()
  {
    return (char*) "IV";
  }
  char five()
  {
    return 'V';
  }
  char* nine()
  {
    return (char*) "IX";
  }
  int multiplier()
  {
    return 1;
  }
};

// ctor for client
RNInterpreter::RNInterpreter()
{
  // use 1-arg ctor to avoid infinite loop
  _thousands = new Thousand(1);
  _hundreds = new Hundred(1);
  _tens = new Ten(1);
  _ones = new One(1);
  std::cout << "RNInterpreter::RNInterpreter: done." << std::endl;
}

RNInterpreter::~RNInterpreter()
{
  // use 1-arg ctor to avoid infinite loop
  if (_thousands)
  {
    delete _thousands;
    _thousands = NULL;
  }
  if (_hundreds)
  {
    delete _hundreds;
    _hundreds = NULL;
  }
  if (_tens)
  {
    delete _tens;
    _tens = NULL;
  }
  if (_ones)
  {
    delete _ones;
    _ones = NULL;
  }
  std::cout << "RNInterpreter::~RNInterpreter: done." << std::endl;
}

// ctor for subclasses, avoids infinite loop
// interpret() for client
int RNInterpreter::interpret(char* input)
{
  int total;
  total = 0;
  _thousands->interpret(input, total);
  _hundreds->interpret(input, total);
  _tens->interpret(input, total);
  _ones->interpret(input, total);
  if (strcmp(input, ""))
  {
    return 0;  // if input was invalid, return 0
  }
  return total;
}

