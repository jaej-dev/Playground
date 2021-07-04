/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 5. Compression
 * Implement a method to perform basic string compression using the counts
 * of repeated characters. For example, the string aabcccccaaa would become
 * a2bc5a3. If the "compressed" string would not become smaller than the 
 * original string, your method should return the original string.
 */
#ifdef __cplusplus
#endif

#include <iostream>
#include <string>

using namespace std;

string CompressByString(string s)
{
  string tmp = "";
  int count = 1;
  size_t i;
  char p = s[0];

  for(i = 1; i <= s.length(); i++)
  {
    if (s[i] == p)
    {
      count++;
    }
    else
    {
      if (count > 1)
      {
        tmp += s[i-1];
        tmp += to_string(count);
      }
      else
      {
        tmp += s[i-1];
      }
      p = s[i];
      count = 1;
    }
  }

  return tmp.length() > s.length() ? s : tmp;
}

int main (void)
{
  const string test1 = "aabccccddd";
  char * test2 = (char *)"deefffgggg";

  cout << "test1 by string = " << test1 << endl;
  cout << "Compression for test1 =  " 
    << CompressByString(test1) << endl;

  cout << "test2 by char   = " << test2 << endl;
  cout << "Compression for test2 =  " 
    << CompressByString(test2) << endl;

  return 0;
}
