/*           
 * Title        :   13. Chain of responsibility
 * Filename     :   Chain_of_responsibility.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * 
 */
#pragma once

#include <iostream> 
#include <string>

class EventHandler
{
public:
  typedef int Event;
  enum EVENT_LIST_E {
    E_NONE = -1,
    E_WIFI = 0x1,
    E_LTE = 0x2,
    E_BT = 0x4,
    E_NFC = 0x8,
    E_MASK = 0xF,
  };

  explicit EventHandler(EventHandler* handler, Event event)
  {
    _successor = handler;
    _event = event;
    std::cout << "EventHandler::EventHandler: done" << std::endl;
  }

  virtual const Event GetResisteredEvent()
  {
    std::cout << "EventHandler::GetResistered: event = " << _event << std::endl;
    return _event;
  }

  virtual void SetHandler(EventHandler* handler, Event event)
  {
    _successor = handler;
    _event = event;
    std::cout << "EventHandler::SetEvent: done." << std::endl;
  }

  virtual void HandleEvent()
  {
    if (_successor != 0)
    {
      _successor->HandleEvent();
    }
    else
    {
      std::cout << "EventHandler::HandleEvent: _successor is 0" << std::endl;
    }
  }

  virtual ~EventHandler()
  {
    std::cout << "EventHandler::~EventHandler: done." << std::endl;
  }

private:
  EventHandler* _successor;
  Event _event;
};

class ModuleEntry : public EventHandler
{
public:
  ModuleEntry(Event e) : EventHandler(0, e) { }
  virtual void HandleEvent() {}
};

class Wifi : public ModuleEntry
{
public:
  Wifi(ModuleEntry* module, Event event) : ModuleEntry(0)
  {
    std::cout << "Wifi::Wifi: done." << std::endl;
    SetHandler(module, event);
  }

  void HandleEvent()
  {
    if ((GetResisteredEvent() & E_MASK) == E_WIFI)
    {
      // Offer event support
      std::cout << "Wifi::HandleEvent: Wifi Event support" 
        << std::endl;
      // call next module
      EventHandler::HandleEvent();
    }
    else
    {
      std::cout << "Wifi:HandleEvent: No support from Wifi" << std::endl;
      EventHandler::HandleEvent();
    }
  }
};

class Lte : public ModuleEntry
{
public:
  Lte(ModuleEntry* module, Event event) : ModuleEntry(0)
  {
    SetHandler(module, event);
    std::cout << "Lte::Lte: done." << std::endl;
  }

  void HandleEvent()
  {
    if ((GetResisteredEvent() & E_MASK) == E_LTE)
    {
      // Offer event support
      std::cout << "Lte::HandleEvent: Lte Event support" 
        << std::endl;
      // call next module
      EventHandler::HandleEvent();
    }
    else
    {
      std::cout << "Lte:HandleEvent: No support from Lte" << std::endl;
      EventHandler::HandleEvent();
    }
  }
};

class Bt : public ModuleEntry
{
public:
  Bt(ModuleEntry* module, Event event) : ModuleEntry(0)
  {
    SetHandler(module, event);
    std::cout << "Bt::Bt: done." << std::endl;
  }

  void HandleEvent()
  {
    if ((GetResisteredEvent() & E_MASK) == E_BT)
    {
      // Offer event support
      std::cout << "Bt::HandleEvent: Bt Event support" 
        << std::endl;
      // call next module
      EventHandler::HandleEvent();
    }
    else
    {
      std::cout << "Bt:HandleEvent: No support from Bt" << std::endl;
      EventHandler::HandleEvent();
    }
  }
};

class Nfc : public ModuleEntry
{
public:
  Nfc(ModuleEntry* module, Event event) : ModuleEntry(0)
  {
    SetHandler(module, event);
    std::cout << "Nfc::Nfc: done." << std::endl;
  }

  void HandleEvent()
  {
    if ((GetResisteredEvent() & E_MASK) == E_NFC)
    {
      // Offer event support
      std::cout << "Nfc::HandleEvent: Nfc Event support" 
        << std::endl;
      // call next module
      EventHandler::HandleEvent();
    }
    else
    {
      std::cout << "Nfc:HandleEvent: No support from Nfc" << std::endl;
      EventHandler::HandleEvent();
    }
  }
};
