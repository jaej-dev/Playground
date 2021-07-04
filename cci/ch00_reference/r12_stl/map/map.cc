/*
 * map.cc:
 */

#include <string>
#include <iostream>
#include <map>

namespace test {

typedef struct _character {
  std::string name;
  int id_num;
  int level;
} character; // charater

void print_table(std::map<std::string, test::character> &c)
{
  std::map<std::string, test::character>::iterator it;

  if(!c.empty()) 
  {
    std::cout << "Total charaters: " << c.size() << "\n";
    for(it = c.begin(); it != c.end(); ++it) 
    {
      std::cout << " name(" << it->second.name << ")  ";
      std::cout << " id_num(" << it->second.id_num << ")  ";
      std::cout << " level(" << it->second.level << ")\n";
    }
  } 
  else 
  {
    std::cout << "total charaters: empty" << "\n";
  }
  std::cout << "\n";
}

void search(std::map<std::string, test::character> &c, const std::string &str)
{
  std::map<std::string, test::character>::iterator it = c.find(str);
  if(it != c.end())
  {
    std::cout << "Found: \n";
    std::cout << " name(" << it->second.name << ")  ";
    std::cout << " id_num(" << it->second.id_num << ")  ";
    std::cout << " level(" << it->second.level << ")\n";
  }
  else
  {
    std::cout << "Can not found: " << str << "\n";
  }
  std::cout << "\n";
}
}  // namespace test

int main(void)
{
  std::map<std::string, test::character> characters;

  // fill 
  test::character any_one;
  any_one.name = "one";
  any_one.id_num = 10;
  any_one.level = 11;
  characters.insert(std::map<std::string, test::character>::value_type(any_one.name, any_one));

  test::character any_two;
  any_two.name = "two";
  any_two.id_num = 20;
  any_two.level = 22;
  characters.insert(std::map<std::string, test::character>::value_type(any_two.name, any_two));

  // show data  
  test::print_table(characters);

  // search
  test::search(characters, "three");
  test::search(characters, "one");

  return 0;
}

