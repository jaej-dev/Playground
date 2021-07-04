/*           
 * Title        :   11. Flyweight Pattern
 * Filename     :   Flyweight.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>
#include <map>

/* Flyweight abstract */
class Character
{
public:
  virtual ~Character() {}
  virtual void Display(int pointSize) = 0;

protected:
  /* _extrinsicState */
  char _symbol;
  int _width;
  int _height;
  int _pointSize;
};

/* ConcreteFlyweight */
class CharacterA : public Character
{
public:
  CharacterA()
  {
    _symbol    = 'A';
    _width     = 120;
    _height    = 100;
    _pointSize = 0; //initialize
    std::cout << "CharacterA:CharacterA: created." << std::endl;
  }
  void Display(int pointSize)
  {
    _pointSize = pointSize;
    std::cout << "CharacterA::Display: " << "(pointsize " << _pointSize 
      << ")\n" << std::endl;
  }
};

class CharacterB : public Character
{
public:
  CharacterB()
  {
    _symbol    = 'B';
    _width     = 140;
    _height    = 100;
    _pointSize = 0; //initialize
    std::cout << "CharacterB:CharacterB: created." << std::endl;
  }
  void Display(int pointSize)
  {
    _pointSize = pointSize;
    std::cout << "CharacterB::Display: " << "(pointsize " << _pointSize 
      << ")\n" << std::endl;
  }
};

//C, D, E,...

class CharacterN : public Character
{
public:
  CharacterN()
  {
    _symbol    = 'N';
    _width     = 100;
    _height    = 100;
    _pointSize = 0; //initialize
    std::cout << "CharacterN:CharacterN: created." << std::endl;
  }
  void Display(int pointSize)
  {
    _pointSize = pointSize;
    std::cout << "CharacterN::Display: " << "(pointsize " << _pointSize 
      << ")\n" << std::endl;
  }
};

/* FlyweightFactory */
class CharacterFactory
{
public:
  CharacterFactory()
  {
    std::cout << "CharacterFactory::CharacterFactory: done." << std::endl;
  }

  virtual ~CharacterFactory()
  {
    while(!characters_.empty())
    {
      std::map<char, Character*>::iterator it = characters_.begin();
      std::cout << "CharacterFactory::~CharacterFactory: deleted character = "
        << it->second << std::endl;
      delete it->second;
      characters_.erase(it);
    }
    std::cout << "CharacterFactory::~CharacterFactory: done." << std::endl;
  }

  Character* GetCharacter(char key)
  {
    Character* character = NULL;
    if(characters_.find(key) != characters_.end())
    {
      character = characters_[key];
    }
    else
    {
      switch(key)
      {
      case 'A':
        character = new CharacterA();
        break;
      case 'B':
        character = new CharacterB();
        break;
      //...
      case 'N':
        character = new CharacterN();
        break;
      default:
        std::cout << "Not Implemented" << std::endl;
        throw("GetCharacter: Not Implemented");
      }
      characters_[key] = character;
    }
    std::cout << "CharacterFactory::GetCharacter: " << character << std::endl;
    return character;
  }
private:
  std::map<char, Character*> characters_;
};

