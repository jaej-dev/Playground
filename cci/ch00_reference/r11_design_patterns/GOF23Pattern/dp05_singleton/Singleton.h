/*           
 * Title        :   05. Singleton
 * Filename     :   Singleton.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * Using C++11 smart pointer
 * See Makefile to use pthread
 *
 * Reference 11. GOF 23 Design Patterns
 */
#pragma once

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include <memory>
#include <mutex>


class Singleton
{
public:
  static std::shared_ptr<Singleton> GetInstance(void)
  {
    static std::shared_ptr<Singleton> instance = NULL;

    if (!instance)
    {
      // critical section
      std::lock_guard<std::mutex> lock(mutex_);
      if (!instance)
        instance.reset(new Singleton);
    }

    //std::cout << "GetInstance: " << instance << std::endl;
    return instance;
  }
  
  static void Demo(std::string name, int count)
  {
    // critical section
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "Demo: " << name << " loop " << count << std::endl;
  }

  ~Singleton(){};

private:
  Singleton(){};
  Singleton(const Singleton&); /* to prevent copy being created of the obj */
  Singleton& operator = (const Singleton&);
  static std::mutex mutex_;
};
