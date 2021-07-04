/*           
 * Title        :   Chapter3 Stacks and Queue
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * 
 * Question 18: set of stack
 * Imagine a (literal) stack of plates. If the stack gets too high, it might 
 * topple. Therefore, in real life, we would likely start a new stack when 
 * the previous stack exceeds some threshold. Implement a _stacks structure 
 * SetOfStacks that mimics this. SetOf-Stacks should be composed of several 
 * stacks and should create a new stack once the previous one exceeds capacity.
 * SetOfStacks.push() and SetOfStacks. pop () should behave identically to a 
 * single stack (that is, pop () should return the same values as it would if 
 * there were just a single stack). How would you design a stack which, 
 * in addition to push and pop, also has a function min which returns the 
 * minimum element? Push, pop and min should all operate in 0(1) time.
 */

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

template <typename T>
class SetOfStack
{
private:
  vector< stack<T> > _stacks;
  int _vectorIndex;
  int _stackSize;

public:
  SetOfStack()
  {
    _stackSize = 10;
   _vectorIndex = -1;
    cout << "SetOfStack: default stack size is 10" << endl;
  }

  SetOfStack(int s)
  {
    _stackSize = s;
   _vectorIndex = -1;
    cout << "SetOfStack: stack size is " << _stackSize << endl;
  }
  
  ~SetOfStack()
  {
    cout << "~SetOfStack: done. " << endl;
  }

  T top(void)
  {
    T ret;
    if (_stacks.size() <= 0)
    {
      cout << "top: vector of stack is empty" << endl;
      return (T) 0;
    }

    ret = _stacks.back().top();
    cout << "top: data= " << ret << endl;
    return ret;
  }

  void push(const T val)
  {
    if (_stacks.empty() || _stacks.back().size() == _stackSize)
    {
      stack<T> newStack;
      newStack.push(val);
      _stacks.push_back(newStack);
      _vectorIndex++;
    cout << "push: created a new stack" << endl;
    }
    else
    {
      _stacks.back().push(val);
    }
    cout << "push: index= " << _vectorIndex << ", data= " << val << endl;
  }

  void pop(void)
  {
    if (_stacks.size() <= 0)
    {
      cout << "pop: vector of stack is empty" << endl;
      return;
    }

    if (_stacks.size() > 0)
    {
      _stacks.back().pop();
      if (_stacks.back().size() == 0)
      {
        _stacks.pop_back();
      _vectorIndex--;
        cout << "pop: deleted stack" << endl;
      }
    }
    cout << "pop: index= " << _vectorIndex << endl;
  }

  void pop_at(int index)
  {
    // vector index start 0, so we have to index + 1
    if (index + 1 > _stacks.size())
    {
      cout << "pop_at: index value is wrong, index= " << index << endl;
      return;
    }

    if (_stacks.size() <= 0)
    {
      cout << "pop_at: vector of stack is empty" << endl;
      return;
    }

    _stacks[index].pop();
     if (_stacks[index].size() == 0)
     {
      _stacks.erase(_stacks.begin() + index);
      _vectorIndex--;
      cout << "pop_at: deleted stack, index= " << index << endl;
     }
    cout << "pop_at: done. index= " << index << endl;
  }
};


/*
 * main entry
 */  
#define TEST_ELEMENTS 9

int main (void)
{
  cout << "\nmain: Initialze stack ----------------------------------" << endl;
  SetOfStack<int> stack(4);

  cout << "\nmain: push some elements -------------------------------" << endl;
  for (int i = 0; i < TEST_ELEMENTS; ++i)
    stack.push(100 + (i * 100));

  cout << "\nmain: pop some elements --------------------------------" << endl;
  for (int i = 0; i < TEST_ELEMENTS+2; ++i)
  {
    stack.top();
    stack.pop();
  }

  cout << "\nmain: push some elements -------------------------------" << endl;
  for (int i = 0; i < TEST_ELEMENTS; ++i)
    stack.push(1000 + (i * 1000));
  
  cout << "\nmain: pop some elements via pop_at ---------------------" << endl;
  stack.pop_at(2);
  stack.pop_at(2);
  stack.top();
  stack.pop_at(1);
  stack.pop_at(1);
  stack.top();
  stack.pop_at(1);
  stack.pop_at(1);
  stack.top();

  cout << "\nmain: Program exit -------------------------------------" << endl;
  return 0;
}
