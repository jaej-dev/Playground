/*           
 * Title        :   15. Interpreter pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Interpreter.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create interpreter -------------------------------" << endl;
  RNInterpreter interpreter;
  
  cout << "\nmain: Run clinet ---------------------------------------" << endl;
  char input[20];
  cout << "Enter Roman Numeral: ";
  cin >> input;
  cout << "   interpretation is " << interpreter.interpret(input) << endl;

  cout << "\nmain: Program exit -------------------------------------" << endl;

  return 0;
}
