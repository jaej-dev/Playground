/*
 * Title      :   xver_test.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <iostream>
#include <string>
#include <xver/xver.h>

int main(int argc, char *argv[]) {
  std::cout << "This version is " << xver_t::version() << std::endl;
  
  std::cout << "Is this version at least 1.0.0? " << xver_t::at_least(1,0,0)
            << std::endl;

  std::cout << "Is this version at least 0.2.0? " << xver_t::at_least(0,2,0)
            << std::endl;

  std::cout << "Is this version at least 0.1.0? " << xver_t::at_least(0,1,0)
            << std::endl;
 
  std::cout << "Is this version at least 0.1.1? " << xver_t::at_least(0,1,1)
            << std::endl;

  return 0;
}
