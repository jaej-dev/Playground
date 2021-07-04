/*           
 * Title        :   Chapter1 Arrays and Strings
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Question 3. Permutation
 * Given two strings, write a method to decide if one is a Permutation of 
 * the other.
 */
#ifdef __cplusplus
#endif

#include <iostream>
#include <string>

using namespace std;


bool TestString()
{
	int letters[10] = {0};
	int i;
	for(i = 0; i<10; ++i )
	{
		cout << letters[i] << endl;
	}
	for(i = 0; i<10; ++i )
	{
		letters[i]++;
	}
	for(i = 0; i<10; ++i )
	{
		cout << letters[i] << endl;
	}
  return 0;
}

bool Permutation(string str1, string str2)
{
	int len1 = str1.length();
	int len2 = str2.length();

	if( len1 != len2 )
		return false;
	
	int letters[256] = {0};
	for(int i = 0; i < len1; i++)
	{
		letters[(int)str1[i]]++;	// count number of each char in str1
		cout << "str1[" << i << "]: " << str1[i] << ", letters[" << i << "]: " 
			<< letters[(int)str1[i]]  << endl;
	}
	
	for(int i = 0; i < len2; i++)
	{
		cout << "str2[" << i << "]: " << str2[i] << ", letters[" << i << "]: " 
			<< letters[(int)str2[i]]  << endl;
		
		if(--letters[(int)str2[i]] < 0)
		{
			return false;
		}
	}

	return true;
}

int main(void)
{
	cout << "Check if one string is a permuation of another." << endl;

	string test1 = "abcde";
	string test2 = "abcdf";
	string test3 = "bcdea";
//	string test1 = "abcdefghijk";
//	string test2 = "abcdefghbjk";
//	string test3 = "gikadecfbhj";

	cout << "test 1: " << test1 << endl;
	cout << "test 2: " << test2 << endl;
	cout << "test 3: " << test3 << endl << endl << endl;

	cout << "test 1 is a Permutation of test 2: " 
		 << ( Permutation(test1, test2) ? "True" : "False" ) << endl << endl;
	cout << "test 1 is a Permutation of test 3: " 
		 << ( Permutation(test1, test3) ? "True" : "False" ) << endl << endl;
	cout << "test 2 is a Permutation of test 3: " 
		 << ( Permutation(test2, test3) ? "True" : "False" ) << endl << endl;

	//TestString(); // how to count number of each char in string
	return 0;
}

