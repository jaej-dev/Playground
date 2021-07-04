/*           
 * Title        :   05. Singleton
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <thread>
#include "Singleton.h"


using namespace std;


std::mutex Singleton::mutex_; /* explicit intiialization might be needed */
static std::shared_ptr<Singleton> gInstance = NULL;

void func1(void)
{
  for (int i = 0; i < 10; ++i)
  {
    //gInstance->GetInstance();
    gInstance->Demo("func1", i);
  }
}

void func2(void)
{
  for (int i = 10; i > 0; --i)
  {
    //gInstance->GetInstance();
    gInstance->Demo("func2", i);
  }
}

/*
 * main entry
 */  
int main (void)
{
  cout << "\nmain: create singleton instance--------------------------" << endl;
  gInstance = Singleton::GetInstance();

  cout << "\nmain: create threads-------------------------------------" << endl;
  thread th1(func1);
  thread th2(func2);

  th1.join();
  th2.join();

  cout << "\nmain: program exit---------------------------------------" << endl;

  return 0;
}

