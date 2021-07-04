/*
 * Title      :   lambda_expression.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <iostream>
#include <memory>
#include <functional>
#include <vector>
#include <array>
#include <algorithm>

class sum {
 public:
  sum() : total_(0) {}
  void operator()(int n) { total_ += n; }
  int get() { return total_; }

 private:
  int total_;
};

std::function<void(void)> lambda_msg() {
  return []() { std::cout << "return lambda expression\n"; };
}

int main(int argc, char* argv[]) {
  std::cout << "program start...\n";

  std::cout << "\n--- lambda expression: std::function ---\n";
  std::function<void (std::string)> show_msg; 
  show_msg = [](std::string str){ std::cout << str << std::endl; };
  show_msg("hello lambda experession with std::function");
  
  std::cout << "\n--- lambda expression: auto ---\n";
  std::vector<int> numbers{ 1, 2, 3, 4, 5 };
  auto print_msg = [](int n){ std::cout << n << std::endl; };
  std::for_each(numbers.begin(), numbers.end(), print_msg);
  sum s = for_each(numbers.begin(), numbers.end(), sum());
  std::cout << "sum: " << s.get() << std::endl;

  std::cout << "\n--- lambda expression: function's parameters ---\n";
  std::function<void(int)> disp_msg([](int n) { std::cout << n << std::endl; });
  std::for_each(numbers.begin(), numbers.end(), [](int &n) { n *= n; });
  std::for_each(numbers.begin(), numbers.end(), disp_msg);
 
  std::cout << "\n--- lambda expression: function's return ---\n";
  auto ret = lambda_msg();
  ret();

  std::cout << "\n--- lambda expression: trailing return type ---\n";
  auto x_fun = []() { return 3.14f;  };
  auto y_fun = []()->float { return 3.141592f;  };
  float x = x_fun();
  float y = y_fun();
  std::cout << "x: " << x << ", y: " << y << std::endl;

  std::cout << "\n--- lambda expression: capture by copy ---\n";
  std::string name = "james";
  [name](){ std::cout << "name: " << name << std::endl; }();
 
  std::cout << "\n--- lambda expression: capture by reference ---\n";
  std::array<int, 5> num_arr = { 1, 2, 3, 4, 5 };
  int total = 0;
  std::for_each(num_arr.begin(), num_arr.end(), [&total](int& n){ total += n; });
  std::cout << "total: " << total << std::endl;

  std::cout << "\nprogram exit...\n";
  return 0;
}
