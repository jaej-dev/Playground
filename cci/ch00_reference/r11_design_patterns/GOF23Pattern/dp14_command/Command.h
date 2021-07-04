/*           
 * Title        :   14. Command pattern
 * Filename     :   Command.h
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

const int MaxCommand = 5;

enum Receiver
{
  LIGHT = 0, FAN, DOOR, NONE
}; 

/* Command Interface */
class Command
{
public:
  virtual ~Command() {}
  virtual void Execute() = 0;
};
 
/* Receiver Class */
class Light 
{
public:
  Light()
  {
    std::cout << "Light::Light: done." << std::endl;
  }

  ~Light()
  {
    std::cout << "Light::~Light: done." << std::endl;
  }

  void On()
  {
    std::cout << "Light::On: The light is turned on" << std::endl;
  }

  void Off()
  {
    std::cout << "Light::Off: The light is turned off" << std::endl;
  }
}; 

class Fan
{
public:
  Fan()
  {
    std::cout << "Fan::Fan: done." << std::endl;
  }

  ~Fan()
  {
    std::cout << "Fan::~Fan: done." << std::endl;
  }
  
  void On()
  {
    std::cout << "Fan::On: The light is turned on" << std::endl;
  }

  void Off()
  {
    std::cout << "Fan::Off: The light is turned off" << std::endl;
  }
}; 

/* Concrete command */
class NullCommand : public Command 
{
public:
  NullCommand()
  { 
    std::cout << "NullCommand::NullCommand: done." << std::endl; 
  }

  ~NullCommand()
  { 
    std::cout << "NullCommand::~NullCommand: done." << std::endl; 
  }
  
  void Execute()
  {
    std::cout << "NullCommand::Execute: does nothing" << std::endl; 
  }
};

class LightOnCommand : public Command 
{
public:
  LightOnCommand(Light* l) : _light(l) 
  {
    std::cout << "LightOnCommand::LightOnCommand: done." << std::endl; 
  }

  ~LightOnCommand()
  {
    std::cout << "LightOnCommand::~LightOnCommand: done." << std::endl; 
  }

  void Execute()
  {
    _light->On();
  }

private:
  Light* _light;
};
 
class LightOffCommand : public Command 
{
public:
  LightOffCommand(Light* l) : _light(l)
  {
    std::cout << "LightOffCommand::LightOffCommand: done." << std::endl; 
  }

  ~LightOffCommand()
  {
    std::cout << "LightOffCommand::~LightOffCommand: done." << std::endl; 
  }

  void Execute()
  {
    _light->Off();
  }

private:
  Light* _light;
};

class FanOnCommand : public Command 
{
public:
  FanOnCommand(Fan* f) : _fan(f)
  {
    std::cout << "FanOnCommand::FanOnCommand: done." << std::endl; 
  }

  ~FanOnCommand()
  {
    std::cout << "FanOnCommand::~FanOnCommand: done." << std::endl; 
  }

  void Execute()
  {
    _fan->On();
  }

private:
  Fan* _fan;
};

class FanOffCommand : public Command 
{
public:
  FanOffCommand(Fan *f) : _fan(f)
  {
    std::cout << "FanOffCommand::FanOffCommand: done." << std::endl; 
  }

  ~FanOffCommand()
  {
    std::cout << "FanOffCommand::~FanOffCommand: done." << std::endl; 
  }

  void Execute()
  {
    _fan->Off();
  }

private:
  Fan* _fan;
};

/* Invoker, stores the concrete command object */
class RemoteControl 
{
public:
  Command *nullCmd = new NullCommand;

  RemoteControl() : _onCmd(MaxCommand), _offCmd(MaxCommand)
  {
    //Command *nullCmd = new NullCommand;
    for(int i = 0; i < MaxCommand; ++i)
    {
      _offCmd[i] = nullCmd;
      _onCmd[i] = nullCmd;
    }
    std::cout << "RemoteControl::RemoteControl: done." << std::endl; 
  }

  ~RemoteControl()
  {
    if (nullCmd)
    {
      delete nullCmd;
    }
    nullCmd = NULL;
    std::cout << "RemoteControl::~RemoteControl: done." << std::endl; 
  }

  void SetCommand(Receiver id, Command* onCmd, Command* offCmd)
  {
    _onCmd[id] = onCmd;
    _offCmd[id] = offCmd;
  }

  void OnButtonPressed(Receiver id)
  {
    _onCmd[id]->Execute();
  } 

  void OffButtonPressed(Receiver id)
  {
    _offCmd[id]->Execute();
  } 

private:
  std::vector<Command*> _onCmd, _offCmd;
};

