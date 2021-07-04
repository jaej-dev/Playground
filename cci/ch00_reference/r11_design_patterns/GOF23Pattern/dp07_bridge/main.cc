/*           
 * Title        :   07. Bridge
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Bridge.h"


using namespace std;


/*
 * main entry
 * client
 */  
int main (void)
{
  cout << "\nmain: red circle  --------------------------------------" << endl;
  RedCircle* redCircle = new RedCircle;
  Shape* redCircleShape = new Circle(10, 100, 100, redCircle);
  redCircleShape->Draw();

  cout << "\nmain: blue circle  -------------------------------------" << endl;
  BlueCircle* blueCircle = new BlueCircle;
  Shape* blueCircleShape = new Circle(20, 300, 400, blueCircle);
  blueCircleShape->Draw();

  cout << "\nmain: black rectangle  --------------------------------" << endl;
  BlackRectangle* blackRectangle = new BlackRectangle;
  Shape* blackRectangleShape = new Rectangle(10, 20, 30, 40, blackRectangle);
  blackRectangleShape->Draw();

  cout << "\nmain: gray rectangle  --------------------------------" << endl;
  GrayRectangle* grayRectangle = new GrayRectangle;
  Shape* grayRectangleShape = new Rectangle(50, 60, 70, 80, grayRectangle);
  grayRectangleShape->Draw();

  // release heap
  delete redCircle;
  delete redCircleShape;
  delete blueCircle;
  delete blueCircleShape;
  delete blackRectangle;
  delete blackRectangleShape;
  delete grayRectangle;
  delete grayRectangleShape;

  cout << "\nmain: program exit -------------------------------------" << endl;

  return 0;
}

