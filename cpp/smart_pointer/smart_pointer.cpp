/*
 * Title      :   smart_pointer.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <iostream>
#include <memory>

class zmsg {
 public:
  explicit zmsg(std::string name) : name_(name) {
    std::cout << this << ": constructed.. " << name_ << std::endl;
  }
  ~zmsg() { std::cout << this << ": destructed.. " << name_ << std::endl; }

  void show_msg(std::string _msg) {
    std::cout << this << ": " << _msg << std::endl;
  }

 private:
  std::string name_;
};

int main(int argc, char* argv[]) {
  std::cout << "program start...\n";
  
  std::cout << "\n--- test for shared pointer ---\n";

  std::shared_ptr<zmsg> zm1(std::make_shared<zmsg>("smart"));
  zm1->show_msg("shared pointer zm1 is created");
  std::cout << "shared pointer count: " << zm1.use_count() << "\n";
 
  auto zm2 = zm1;
  zm2->show_msg("shared pointer zm2 is referenced from zm1");
  std::cout << "shared pointer count: " << zm1.use_count() << "\n";
  
  auto zm3 = zm1;
  zm3->show_msg("shared pointer zm3 is referenced from zm1");
  std::cout << "shared pointer count: " << zm1.use_count() << "\n";
  
  zm1->show_msg("shared pointer zm1 is alive");
  zm2->show_msg("shared pointer zm2 is alive");
  zm3->show_msg("shared pointer zm3 is alive");

  zm3.reset();
  std::cout << "reset zm3, shared pointer count: " << zm1.use_count() << "\n";
  zm2.reset();
  std::cout << "reset zm2, shared pointer count: " << zm1.use_count() << "\n";
  zm1.reset();
  std::cout << "reset zm1, shared pointer count: " << zm1.use_count() << "\n";
  
  zm1->show_msg("shared pointer zm1 is alive");
  zm2->show_msg("shared pointer zm2 is alive");
  zm3->show_msg("shared pointer zm3 is alive");
  
  std::cout << "\n--- test for unique pointer ---\n";
  //std::unique_ptr<zmsg> zm2(std::make_unique<zmsg>("pointer")); // c++14
  std::unique_ptr<zmsg> zm5(new zmsg("pointer")); // c++11
  zm5->show_msg("unique pointer zm5 is created");

  auto zm6 = std::move(zm5);
  zm6->show_msg("unique pointer zm5 is moved to zm6");
  std::cout << "zm6 address: " << zm6.get() << std::endl;

  //auto zm7 = zm6; // it makes occur compile error
  
  zm5->show_msg("unique pointer zm5 is alive");
  zm6->show_msg("unique pointer zm6 is alive");

  zm5.reset();
  zm6.reset();

  zm5->show_msg("unique pointer zm5 is alive");
  zm6->show_msg("unique pointer zm6 is alive");

  std::cout << "\nprogram exit...\n";
  return 0;
}
