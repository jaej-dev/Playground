/*
 * shared_ptr.cc:
 */

#include <iostream>
#include <vector>
#include <memory>

namespace test {
class sptest
{
public:
  sptest()
  {
    val = 0;
    std::cout << "Constructor\n";
  }
  ~sptest()
  {
    std::cout << "Destructor\n";
  }
  void SetVal(int num)
  {
    val = num;
  }
  const int GetVal()
  {
    return val;
  }

private:
  int val;
};
}

int main(void)
{
  std::vector<std::shared_ptr<test::sptest>> sp_v;

  for(int i = 0; i < 3; ++i)
  {
    std::shared_ptr<test::sptest> sp(new test::sptest());
    sp_v.push_back(sp);
  }

  for(int i = 0; i < 3; ++i)
  {
    std::cout << sp_v.at(i) << "\n";
  }

  return 0;
}

