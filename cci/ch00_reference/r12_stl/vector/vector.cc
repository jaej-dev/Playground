/*
 * vector.cc:
 */

#include <vector> 
#include <iostream>

namespace vector_test {
int g_cnt;
}

void print_table(std::vector<int> &vec)
{
  if(vec.empty())
  {
    std::cout << "table is empty..." << std::endl; 
    return;
  }
  
  std::cout << "[" << vector_test::g_cnt++ << "] ";
  for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}

int main(void)
{
  vector_test::g_cnt = 0;

  std::vector<int> q;
  q.push_back(10); 
  q.push_back(11); 
  q.push_back(12);
           
  std::vector<int> v;
  for(int i=0; i<5; ++i) {
    v.push_back(i);
  }
  print_table(v); // v contains 0 1 2 3 4

  std::vector<int>::iterator it = v.begin() + 1;
  // insert 33 before the second element:
  it = v.insert(it, 33);
  print_table(v);  // v contains 0 33 1 2 3 4
  
  // it points to the inserted element
  //insert the contents of q before the second element:
  v.insert(it, q.begin(), q.end());
  print_table(v);  // v contains 0 10 11 12 33 1 2 3 4
  
  // iterator 'it' is invalid
  it = v.begin() + 3;
  // it points to the fourth element of v
  // insert three time -1 before the fourth element:
  v.insert(it, 3, -1);
  print_table(v);  // v contains 0 10 11 -1 -1 -1 12 33 1 2 3 4
  
  // iterator 'it' is invalid
  // erase the fifth element of v
  it = v.begin() + 4;
  v.erase(it);
  print_table(v);  // v contains 0 10 11 -1 -1 12 33 1 2 3 4
  
  // iterator 'it' is invalid
  // erase the second to the fifth element:
  it = v.begin() + 1;
  v.erase(it, it + 4);
  // v contains 0 12 33 1 2 3 4
  print_table(v); 

  // iterator 'it' is invalid
  // clear all of v's elements
  v.clear();
  print_table(v);

  return 0;
}
