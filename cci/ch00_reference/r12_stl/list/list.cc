/*
 * Create a random integer list and sort the list
 */

#include <cstdlib>
#include <iostream>
#include <list>

namespace list_test {
int g_cnt;
}

void print_table(std::list<int> &lst)  
{
  if(lst.empty())
  {
    std::cout << "table is empty..." << std::endl;
    return;
  }
  
  std::cout << "[" << list_test::g_cnt++ << "] ";
  for(std::list<int>::iterator it = lst.begin(); it != lst.end(); ++it) 
  {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}


int main()
{
  std::list<int> lst;

  //create a list of random integers
  for(int i = 0; i < 10; ++i)
  {
    lst.push_back(rand()%100 + 10);
  }
  print_table(lst);        

  // create an iterator object and make it point to the beginning of the list
  std::list<int>::iterator it = lst.begin();
  // insert 
  lst.push_front(1);
  lst.push_front(2);
  lst.push_front(3);
  print_table(lst);

  // pop
  it = lst.end();
  lst.pop_back();
  lst.pop_back();
  lst.pop_back();
  print_table(lst);


  // sort the list
  lst.sort();
  print_table(lst);

  // clear list
  lst.clear();
  print_table(lst);

  return (0);
}
