/*
 * Title      :   forward.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <iostream>
#include <memory>

class value {
 public:
   value(int&& n) { std::cout << "rvalue overload: " << n << std::endl; }
   value(int&  n) { std::cout << "lvalue overload: " << n << std::endl; }

};

template<typename x_t, typename y_t>
std::unique_ptr<x_t> make_unique_ptr(y_t&& y) {
  return std::unique_ptr<x_t>(new x_t(std::forward<y_t>(y)));
}

int main(int argc, char* argv[]) {
  std::cout << "program start...\n";
  
  std::cout << "\n--- test for rvalue case 1---\n";
  auto p1 = make_unique_ptr<value>(7);

  std::cout << "\n--- test for rvalue case 2---\n";
  auto j = make_unique_ptr<value>(7);

  std::cout << "\n--- test for lvalue case 1---\n";
  int k = 9;
  auto p3 = make_unique_ptr<value>(k);

  std::cout << "\nprogram exit...\n";
  return 0;
}
