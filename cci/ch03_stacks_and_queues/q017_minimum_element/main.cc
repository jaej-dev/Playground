/*           
 * Title        :   Chapter3 Stacks and Queue
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * 
 * Question 17: minimum element
 * How would you design a stack which, in addition to push and pop, also has a
 * function min which returns the minimum element? Push, pop and min should all
 * operate in 0(1) time.
 * 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
//#include "circulardoublylist.h"
//#include "stack.h"

using namespace std;


template <typename T>
class SingleArrayStack
{
  public:
    SingleArrayStack();
    SingleArrayStack(int size);
    ~SingleArrayStack();
    void Push(T data);
    T Pop(void);
    T Min(void);
    void PrintAllStack(void);

  private:
    int _size;
    T * _oriBuf;
    T * _minBuf;
    int _oriTop;
    int _minTop;
};

template <typename T>
SingleArrayStack<T>::SingleArrayStack()
{
  _size = 0;
  _oriBuf = NULL;
  _minBuf = NULL;
  _oriTop = -1;
  _minTop = -1;
  cout << "SingleArrayStack: done" << endl;
}

template <typename T>
SingleArrayStack<T>::SingleArrayStack(int size)
{
  _size = size;
  _oriBuf = new T[_size];
  _minBuf = new T[_size];
  for (int i = 0; i < _size; ++i)
  {
    _oriBuf[i] = 0;
    _minBuf[i] = 0;
  }
  _oriTop = -1;
  _minTop = -1;

  cout << "SingleArrayStack: size= " << _size << endl;
}

template <typename T>
SingleArrayStack<T>::~SingleArrayStack()
{
  if (_size < 1)
    return;

  delete[] _oriBuf;  _oriBuf = NULL;
  delete[] _minBuf;  _minBuf = NULL;

  cout << "~SingleArrayStack: done" << endl;
}

template <typename T>
void SingleArrayStack<T>::Push(T data)
{
  if (this->_oriTop == -1 && this->_minTop == -1)
  {
    ++_oriTop;
    ++_minTop;
    this->_oriBuf[_oriTop] = data;
    this->_minBuf[_minTop] = data;
  }
  else
  {
    if (this->_oriTop == (this->_size - 1))
    {
      cout << "SingleArrayStack::Push: this stack is full" << endl;
      return;
    }
    else
    {
      // for origin buff
      ++_oriTop;
      this->_oriBuf[_oriTop] = data;
      // for minimum buff
      if (data < this->_minBuf[this->_minTop])
      {
        ++_minTop;
        this->_minBuf[_minTop] = data;
      }
    }
  }
  cout << "SingleArrayStack::Push: _oriTop= " << _oriTop << ", _oriBuf= " 
    << _oriBuf[_oriTop] << ", _minTop= " << _minTop << ", _minBuf= " 
    << _minBuf[_minTop] << endl;
}

template <typename T>
T SingleArrayStack<T>::Pop(void)
{
  if (this->_oriTop == -1)
  {
    cout << "SingleArrayStack::Pop: this stack is empty" << endl;
    return (T) 0;
  }
  else
  {
    T ret = this->_oriBuf[this->_oriTop];
    --this->_oriTop;
    if(this->_minBuf[this->_minTop] == ret)
    {
      --this->_minTop;
    }
    return (T) ret;
  }
}

template <typename T>
T SingleArrayStack<T>::Min(void)
{
  if (this->_minTop == -1)
  {
    cout << "SingleArrayStack::Min: this stack is empty" << endl;
    return (T) 0;
  }
  else
  {
    T ret = this->_minBuf[this->_minTop];
    cout << "SingleArrayStack::Min: " << ret << endl;
    return ret; 
  }
}

template <typename T>
void SingleArrayStack<T>::PrintAllStack(void)
{
  cout << "PrintAllStack: origin buff:" << endl;
  for (int i = 0; i < _size; ++i)
  {
    cout << "index = " << i << ", data = " << _oriBuf[i] << endl;
  }
  cout << "PrintAllStack: mininum buff:" << endl;
  for (int i = 0; i < _size; ++i)
  {
    cout << "index = " << i << ", data = " << _minBuf[i] << endl;
  }
}

/*
 * main entry
 */  

int main (void)
{
  
  cout << "Step1: Initialze stack, size = 10 ------------------------" << endl;
  SingleArrayStack<int> stack(10);

  cout << "Step2: Push and minumum data" << endl;
  stack.Push(500);
  stack.Min();
  stack.Push(600);
  stack.Min();
  stack.Push(400);
  stack.Min();
  stack.Push(700);
  stack.Min();
  stack.Push(300);
  stack.Min();

  cout << "Step3: Print stack" << endl;
  stack.PrintAllStack();

  cout << "main: program exit ---------------------------------------" << endl;

  return 0;
}
