/*           
 * Title        :   Chapter3 Stacks and Queues
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * 
 * Question 16: three stacks
 * Describe how you could use a single array to implement three stacks.
 * 
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>

using namespace std;


template <typename T>
class SingleArrayStack
{
  public:
    SingleArrayStack();
    SingleArrayStack(int size, int num);
    ~SingleArrayStack();
    void Push(int num, T data);
    T Pop(int num);
    void PrintAllStack(void);

  private:
    int _size;
    int _num;
    T * _buf;
    int * _top;
};

template <typename T>
SingleArrayStack<T>::SingleArrayStack()
{
  _size = 0;
  _num = 0;
  _buf = NULL;
  _top = NULL;
  cout << "SingleArrayStack: done" << endl;
}

template <typename T>
SingleArrayStack<T>::SingleArrayStack(int size, int num)
{
  _size = size;
  _num = num;
  _buf = new T[_size * _num];
  for (int i = 0; i < _num*_size; ++i)
    _buf[i] = 0;
  _top = new int[_num];
  for (int i = 0; i < _num; ++i)
    _top[i] = -1;
  cout << "SingleArrayStack: size= " << size << ", num= " << num << endl;
}

template <typename T>
SingleArrayStack<T>::~SingleArrayStack()
{
  if (_size < 1)
    return;

  delete[] _buf;  _buf = NULL;
  delete[] _top;  _top = NULL;

  cout << "~SingleArrayStack: done" << endl;
}

template <typename T>
void SingleArrayStack<T>::Push(int num, T data)
{
  if (num > _num-1)
  {
    cout << "Push: stack number is wrong." << endl;
    return;
  }

  if (_top[num]+1 >= _size)
  {
    cout << "Push: stack[" << num << "] is full." << endl;
    return;
  }
 
  _top[num]++;
  int offset = (_size * num) + _top[num];
  _buf[offset] = data;
  cout << "Push: offset = " << offset << ", data = " << data << endl;
}

template <typename T>
T SingleArrayStack<T>::Pop(int num)
{
  if (num > _num-1)
  {
    cout << "Pop: stack number is wrong." << endl;
    return (T) 0;
  }

  if (_top[num] == -1)
  {
    cout << "Pop: stack[" << num << "] is empty." << endl;
    return (T) 0;
  }

  int offset = (num*_size) + _top[num];
  T ret = _buf[offset];
  _buf[offset] = 0;
  _top[num]--;
  cout << "Pop: offset = " << offset << ", data = " << ret << endl;
  return (T) ret;
}

template <typename T>
void SingleArrayStack<T>::PrintAllStack(void)
{
  cout << "PrintAllStack:" << endl;
  for (int i = 0; i < _num; ++i)
    for (int j = 0; j < _size; ++j)
    {
      int offset = (i*_size) + j;
      cout << "num = " << i << ", data = " << _buf[offset] << endl;
    }
}

/*
 * main entry
 */  
int main (void)
{
  cout << "Step 1: Initialize stack, size = 3, num = 3" << endl;
#if 0
  int size, num;
  cout << "input stack size: ";
  cin >> size;
  cout << "input stack number: ";
  cin >> num;
#endif  
  SingleArrayStack<int> stack(3, 3);

  cout << "Step 2: push data" << endl;
  stack.Push(0,100);
  stack.Push(0,200);
  stack.Push(0,300);
  stack.Push(0,333);
  stack.Push(1,400);
  stack.Push(1,500);
  stack.Push(1,600);
  stack.Push(1,666);
  stack.Push(2,700);
  stack.Push(2,800);
  stack.Push(2,900);
  stack.Push(2,999);

  cout << "Step 3: print all stack" << endl;
  stack.PrintAllStack();  

  cout << "Step 4: pull data" << endl;
  stack.Pop(0);
  stack.Pop(0);
  stack.Pop(1);
  stack.Pop(1);
  stack.Pop(2);
  stack.Pop(2);
  stack.Pop(2);
  stack.Pop(2);

  cout << "Step 5: print all stack" << endl;
  stack.PrintAllStack();  

  cout << "Step 6: program exit" << endl;

  return 0;
}
