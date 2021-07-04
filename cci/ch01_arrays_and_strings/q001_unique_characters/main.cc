/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 1. Unique characters
 * Implement an algorithm to determine if a string has all unique characters. 
 * What if you cannot use additional data structures?
 */

#ifdef __cplusplus
#endif

#include <iostream>

using namespace std;


int IsUniqueChars(string str)
{
	int checker = 0;  /* 0 means 'a'  */

	for (int i = 0; i < (int)str.length(); ++i)
	{
  	int val = str.at(i) - 'a';
		cout << str.at(i) << " - " << val << endl;
		if ((checker & (1 << val)) > 0)
		{
			return false;
		}
		checker |= (1 << val);	// check by bit vector from 'a' to 'z'
    cout << "checker = hex " << hex << checker << endl;
	}
	return true;
}

int main(void)
{
	string str1, str2;
	str1 = "asdfghjkla";	// false case
	str2 = "asdfghjkl";		// true case

	cout << "Input string1: " << str1 << endl;

	if ( IsUniqueChars(str1) )
		cout << "true" << endl;
	else
		cout << "false" << endl;
	
	cout << "Input string2: " << str2 << endl;

	if ( IsUniqueChars(str2) )
		cout << "true" << endl;
	else
		cout << "false" << endl;

  return 0;
}

