/*           
 * Title        :   08. Composite
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Composite.h"


using namespace std;


/*
 * main entry
 * client
 */  
int main(void)
{
  cout << "\nmain: create component and draw ------------------------" << endl;
  Line line;
  line.Draw();
  Circle circle;
  circle.Draw();
  Rectangle rectangle;
  rectangle.Draw();

  cout << "\nmain: create composite and draw ------------------------" << endl;
  Picture pic;
  pic.Add(&line);
  pic.Add(&circle);
  pic.Add(&rectangle);
  pic.Remove(&line);
  pic.Draw();

  cout << "\nmain: program exit -------------------------------------" << endl;
  
  return 0;
}
