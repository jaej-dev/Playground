/*           
 * Title        :   Cpp Template Example
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 7. Template meta programming
 * 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>

using namespace std;

/* value interface */
struct true_type {
  enum _value {
    value = true,
  };
};

struct false_type {
  enum _value {
    value = false,
  };
};

/* remove const keyword*/
template <typename T>
struct remove_const {
  typedef T type;
};

template <typename T>
struct remove_const <const T> {
  typedef T type;
};

/* to check integral type*/
template <typename T>
struct is_integral_base : false_type {};

#define _IS_INTEGRAL_SPECIALIZATION_(x)  \
  template <> struct is_integral_base<x> : true_type {}

_IS_INTEGRAL_SPECIALIZATION_(signed int);
_IS_INTEGRAL_SPECIALIZATION_(signed short);
_IS_INTEGRAL_SPECIALIZATION_(signed long);
_IS_INTEGRAL_SPECIALIZATION_(signed long long);
_IS_INTEGRAL_SPECIALIZATION_(unsigned int);
_IS_INTEGRAL_SPECIALIZATION_(unsigned short);
_IS_INTEGRAL_SPECIALIZATION_(unsigned long);
_IS_INTEGRAL_SPECIALIZATION_(unsigned long long);
_IS_INTEGRAL_SPECIALIZATION_(unsigned char);
//_IS_INTEGRAL_SPECIALIZATION_(char16_t);
//_IS_INTEGRAL_SPECIALIZATION_(char32_t);
//_IS_INTEGRAL_SPECIALIZATION_(wchar_t);

template <typename T>
struct is_integral : is_integral_base<typename remove_const<T>::type> {};

int main (void)
{
  cout << "int: " << is_integral<int>::value << endl;
  cout << "const unsigned long long: " 
    << is_integral<const unsigned long long>::value << endl;
  cout << "int *: " << is_integral<int *>::value << endl;
  cout << "float: " << is_integral<float>::value << endl;
  cout << "char *: " << is_integral<char *>::value << endl;

  int x = 777;
  cout << "777: " << (x == 777) << endl;

  return 0;
}
