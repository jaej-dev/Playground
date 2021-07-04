/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 6. Matrix rotation
 * Given an image represented by an NxN matrix, where each pixel in the image 
 * is 4 bytes, write a method to rotate the image by 90 degrees. Can you do 
 * this in place?
 *
 * EXAMPLE:
 * 11 12 13 14  90 degree clockwise    23 19 15 11 
 * 15 16 17 18          ---->          24 20 16 12
 * 19 20 21 22                         25 21 17 13
 * 23 24 25 26                         26 22 18 14 
 *
 * view of the row and column, e.g. array[column][row]
 * 00 01 02 03  90 degree clockwise    30 20 10 00
 * 10 11 12 13          ---->          31 21 11 01
 * 20 21 22 23                         32 22 12 02
 * 30 31 32 33                         33 23 13 03
 *
 * hint:
 * http://makthum.com/webdevelopment/softwaredevelopment/Cracking-Coding%20Interview-Array1_6/
 * 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>

using namespace std;

const int MATRIX_N = 4;

void PrintMatrix (int m[][MATRIX_N])
{
  size_t i, j;

  for (i = 0; i < MATRIX_N; i++)
  {
    for (j = 0; j < MATRIX_N; j++)
      cout << m[i][j] << " ";
    cout << endl;
  }
}

void RotateMatrixByAnotherArray (int input[][MATRIX_N], int output[][MATRIX_N])
{
  int column, row, cval, rval;

  for (row = 0; row < MATRIX_N; ++row)
  {
    for (column = 0; column < MATRIX_N; column++)
    {
      rval = MATRIX_N - 1 - column;
      cval = row;
      output[row][column] = input[rval][cval];
    }
  }
}

void RotateMatrixBySwap(int m[][MATRIX_N])
{
  for (int layer = 0; layer < MATRIX_N / 2; ++layer)
  {
    int first = layer; 
    int last = MATRIX_N - 1 - layer;
    for(int i = first; i < last; ++i)
    {
      int offset = i - first;
      int top = m[first][i];                          // step1: save top
      m[first][i] = m[last-offset][first];            // step2: left -> top
      m[last-offset][first] = m[last][last - offset]; // step3: bottom -> left
      m[last][last - offset] = m[i][last];            // step4: right -> bottom
      m[i][last] = top;                               // step5: saved top -> right 
    }
  }
}

int main (void)
{
  int ori[][MATRIX_N] = {
    {11,12,13,24},
    {15,16,17,18},
    {19,20,21,22},
    {23,24,25,26},
  };
  int result[][MATRIX_N] = {
    {0,0,0,0,},
    {0,0,0,0,},
    {0,0,0,0,},
    {0,0,0,0,},
  };

  cout << "Origin matrix: " << endl;
  PrintMatrix(ori);

  cout << "Rotated matrix by another array:" << endl;
  RotateMatrixByAnotherArray(ori, result);
  PrintMatrix(result);

  cout << "Rotated matrix by swap:" << endl;
  RotateMatrixBySwap(ori);
  PrintMatrix(ori);

  return 0;
}
