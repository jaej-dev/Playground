/*
 * set.cc:
 */

#include <string>
#include <iostream>
#include <set>

namespace test {

}  // namespace test

int main(void)
{
  std::set<std::string> s;

  // fill 
  s.insert("gggggg_1");
  s.insert("fffff__2");
  s.insert("a______3");
  s.insert("bb_____4");
  s.insert("ccc____5");
  s.insert("dddd___6");
  s.insert("eeee___7");

  std::cout << "Set contains:\n";

  while (!s.empty()) {
    std::cout << *s.begin() << " ";
    s.erase(s.begin());
  }

  std::cout << "\n";
  return 0;
}

