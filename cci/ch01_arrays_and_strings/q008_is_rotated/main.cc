/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 8. Is rotated
 * Assume you have a method isSubstring which checks if one word is a
 * substring of another. Given two strings, si and s2, write code to check if 
 * s2 is a rotation of s1 using only one call to isSubstring 
 * (e.g.,"waterbottle"is a rotation of "erbottlewat").
 * 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include <cstring>

using namespace std;


bool IsSubString(string s1, string s2)
{
  if (s1.find(s2) != string::npos)
    return true;
  else
    return false;
}

bool IsRotated(string s1, string s2)
{
  unsigned int len = s1.length();

  if (len == s2.length() && len > 0)
  {
    string s1s1 = s1 + s1;
    return IsSubString(s1s1, s2);
  }

  return false;
}

int main (void)
{
  string s1 = "waterbottle";
  string s2 = "erbottlewat";
  string s3 = "erbottlewtt";
  string ret1 = IsRotated(s1, s2) ? "Yes" : "No";
  string ret2 = IsRotated(s1, s3) ? "Yes" : "No";

  cout << "String S1: " << s1 << ", String S2: " << s2 << endl;
  cout << "Is S2 a rotated substring of S1 ? " << ret1 << endl;

  cout << "String S1: " << s1 << ", String S3: " << s3 << endl;
  cout << "Is S3 a rotated substring of S1 ? " << ret2 <<endl;

  return 0;
}
