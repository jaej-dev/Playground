/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 2. Reverse
 * Implement a function void reverse(char* str) in C or C++ which reverses 
 * a nullterminated string.
 */

#ifdef __cplusplus
#endif

#include <iostream>

using namespace std;


string ReverseString(string str)
{
	string tmp = "";

	for(int i = str.length() - 1; i >= 0; i--)
	{
		tmp += str[i];
		cout << "...letter " << i << ": " << str[i] << endl;
	}

	return tmp;
}

int main(void)
{
	string tmp = "abcdefghjkllmnopqrstuvwxyz";

	cout << "Input String: " << tmp << endl;
	cout << "Output String: " << ReverseString(tmp) << endl;

	return 0;
}
