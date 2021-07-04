/*
 * Title      :   implicit_conversion.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <iostream>
#include <memory>

class implicit_conversion {
 public:
  operator float() { return static_cast<float>(var_); }

 private:
  static const int var_ = 10;
};

int main(int argc, char* argv[]) {
  std::cout << "program start...\n";

  float result = 0.0f;
  const float pi = 3.141592f;

  result = pi * implicit_conversion();

  std::cout << "pi: " << pi << ", pi*implicit_conversion(): " << result << "\n";

  std::cout << "program exit...\n";
  return 0;
}
