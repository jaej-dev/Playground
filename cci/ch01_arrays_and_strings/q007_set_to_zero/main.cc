/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 7. Set to zero
 * Write an algorithm such that if an element in an MxN matrix is 0, 
 * its entire row and column are set to 0.
 * 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

const int M_R = 3; // row
const int M_C = 4; // column

void PrintMatrix (int m[][M_C])
{
  int r, c;

  for (r = 0; r < M_R; ++r)
  {
    for (c = 0; c < M_C; ++c)
      cout << m[r][c] << " ";
    cout << endl;
  }
}

void SetToZero(int m[][M_C])
{
  bool rowz[M_R] = {0,};
  bool colz[M_C] = {0,};
  
  for (int r = 0; r < M_R; ++r)
  {
    for (int c = 0; c < M_C; ++c)
    {
      if (m[r][c] == 0)
      {
        rowz[r] = true; // detected zero flag
        colz[c] = true;
      }
    }
  }
 
  for (int r = 0; r < M_R; ++r)
  {
    for (int c = 0; c < M_C; ++c)
    {
      if (rowz[r] || colz[c])
        m[r][c] = 0;
    }
  }
}

int main (void)
{
  int ori[M_R][M_C] = {
    {1,2,3,4},
    {5,6,0,8},
    {9,1,2,0},
  };
  
  cout << "Origin matrix: " << endl;
  PrintMatrix(ori);

  cout << "Set to zero matrix:" << endl;
  SetToZero(ori);
  PrintMatrix(ori);

  return 0;
}
