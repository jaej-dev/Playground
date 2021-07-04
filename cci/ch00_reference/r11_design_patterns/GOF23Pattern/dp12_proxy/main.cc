/*           
 * Title        :   12. Proxy pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Proxy.h"

using namespace std;

/*
 * main entry
 * client
 */  
int main(void)
{
  cout << "\nmain: Create proxy and run -----------------------------" << endl;
  // Create math proxy
  MathProxy proxy;

  //Do the math
  cout << "main: 4 + 2 = " << proxy.Add(4, 2) << endl;
  //cout << "main: 4 - 2 = " << proxy.Sub(4, 2) << endl;
  //cout << "main: 4 * 2 = " << proxy.Mul(4, 2) << endl;
  //cout << "main: 4 / 2 = " << proxy.Div(4, 2) << endl;

  cout << "\nmain: Program exit -------------------------------------" << endl;
  
  return 0;
}
