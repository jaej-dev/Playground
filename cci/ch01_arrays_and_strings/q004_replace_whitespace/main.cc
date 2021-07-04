/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 4. Replace whitespace
 * Write a method to replace all spaces in a string with'%20'. You may assume
 * that the string has sufficient space at the end of the string to hold the 
 * additional characters, and that you are given the "true" length of the string. 
 * (Note: if implementing in Java, please use a character array so that you can 
 * perform this operation in place.)
 *
 * EXAMPLE
 * Input:  "Mr John Smith    "
 * Output: "Mr%20Dohn%20Smith"
 */
#ifdef __cplusplus
#endif

#include <iostream>
#include <cstring>

using namespace std;

//#define ENABLE_DEBUG

string ReplaceSpaceByString(string s)
{
  string tmp = "";

  for(size_t i = 0; i < s.length(); i++)
  {
    if(s[i] == ' ')
      tmp += "%20";
    else
      tmp += s[i];
  }

  return tmp;
}

/*  calculate size of new char via char method */
int CalcSizeOfNewChar(char * s)
{
  char * iter = s;
  int space_len = 0, letter_len = 0, ret = 0;

  while (*iter)
  {
#ifdef ENABLE_DEBUG 
   cout << __func__ << ": iterator = " << *iter << endl;
#endif

    if (*iter == ' ') // space = 0x20
      space_len++;
    else
      letter_len++;
    iter++;
  }

  ret = letter_len + space_len + 1;

#ifdef ENABLE_DEBUG
  cout << __func__ << ": letter= " << letter_len << ", space= " 
    << space_len << ", ret= " << ret << endl;
#endif

  return  ret;
}

int ReplaceSpaceByChar(char * in, int len, char* out)
{
  int i, j;
  for (i = 0, j = 0; i < len-1; ++i, ++j)
  {
    if (in[i] == (char)' ')
    {
      out[j] = '%';
      out[++j] = '2';
      out[++j] = '0';
    }
    else
      out[j] = in[i];
#ifdef ENABLE_DEBUG
    cout << __func__ << ": " << i << ": " << out << endl;  
#endif
  }
  out[j] = '\0';

  return 0;
}

int main (void)
{
  string test1 = "S T R I N G";
  char * test2 = (char *)"C H A R";

  cout << "test1 by string = " << test1 << endl;
  cout << "Replace white space with '%20' for test1 =  " 
    << ReplaceSpaceByString(test1) << endl << endl;

  int size = CalcSizeOfNewChar(test2);
  char * tmp = new char[size];
  ReplaceSpaceByChar(test2, size, tmp);

  cout << "test2 by char   = " << test2 << endl;
  cout << "Replace white space with '%20' for test2 =  " 
    << tmp << endl;

  delete [] tmp;

  return 0;
}
