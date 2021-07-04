/*           
 * Title        :   22. Template method pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "TemplateMethod.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Math operation with x(10) operation(+) y(20) -----" << endl;
  map<string, string> myInput, myOutput;
  myInput.insert(make_pair("x", "10"));
  myInput.insert(make_pair("y", "20"));
  myInput.insert(make_pair("operation", "+"));

  MathOperation a;
  a.Solve(myInput, myOutput);
  cout << myOutput["result"] << endl;

  myInput.clear();
  myOutput.clear();


  cout << "\nmain: List operation with 1 2 3 4 5 --------------------" << endl;
  // 1 2 3 4 5 -> 5 4 3 2 1
  myInput["array"] = "1 2 3 4 5";
  ListOperation b;
  b.Solve(myInput, myOutput);
  cout << myOutput["result"] << endl;

  cout << "\nmain: Program exit -------------------------------------" << endl;

  return 0;
}
