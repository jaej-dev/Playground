/*           
 * Title        :   Linked Lists Example
 * Filename     :   stack.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from 
 * /cci/Ch00_Reference/r03_circular_doubly_linked_list.
 * /cci/Ch00_Reference/r06_stack
 * 
 * Question 15: palindrome
 * Implement a function to check if a linked list is a palindrome.
 * 
 * EXAMPLE:
 * 0 -> 1 -> 2 -> 1 -> 0
 */
#ifndef STACK_H
#define STACK_H

#define DEBUG_ENABLED

#include <iostream>

template <typename T>
class SingleNode
{
  public: 
    SingleNode() : _data(0), _prev(NULL) {}
    SingleNode(T data) : _data(data), _prev(NULL) {}
    ~SingleNode() { _data = 0; _prev = NULL; }
    T GetData();
    void SetData(T);
    SingleNode<T> * GetPrev();
    void SetPrev(SingleNode<T> *);

  private:
    T _data;
    SingleNode<T> * _prev;
};

template <typename T>
class DynamicStack
{
  public:
    DynamicStack() : _top(NULL), _nodeCount(0) {}
    DynamicStack(T data) : _top(new SingleNode<T>(data)) {}
    ~DynamicStack();
    void Push(T);
    T Pop(void);
    bool IsEmpty(void);
    void PrintStack(void);
    void ClearStack(void);

  private:
    SingleNode<T> * _top;
    size_t _nodeCount;
};

/* SingleNode implementation */
template <typename T>
T SingleNode<T>::GetData(void)
{
  return _data;
}

template <typename T>
void SingleNode<T>::SetData(T data)
{
  _data = data;
}

template <typename T>
SingleNode<T> * SingleNode<T>::GetPrev(void)
{
  return _prev;
}

template <typename T>
void SingleNode<T>::SetPrev(SingleNode<T>* prev)
{
  _prev = prev;
}

/* DynamicStack implementation */

template <typename T>
DynamicStack<T>::~DynamicStack()
{
  DynamicStack<T>::ClearStack();
#ifdef DEBUG_ENABLED
  std::cout << "~DynamicStack: done " << std::endl;
#endif
}

template <typename T>
void DynamicStack<T>::Push(T data)
{
  // create new node
  SingleNode<T> * node = new SingleNode<T>();
  node->SetData(data);
  node->SetPrev(NULL);

  // append new node
  if (_top != NULL)
  {
    node->SetPrev(_top);
    _top = node;
  }
  else
  {
    _top = node;
  }

  _nodeCount++;
#ifdef DEBUG_ENABLED
  std::cout << "Push: no= " << _nodeCount << ", addr= " << node 
    << " data= " << node->GetData() << ", prev= " << node->GetPrev() << std::endl;
#endif
}

template <typename T>
T DynamicStack<T>::Pop(void)
{
  SingleNode<T> * prev;
  T ret;

  if (_top == NULL)
  {
#ifdef DEBUG_ENABLED
  std::cout << "Pop: empty" << std::endl;
#endif
    return (T)-1;
  }

  ret = _top->GetData();
  prev = _top->GetPrev();
#ifdef DEBUG_ENABLED
  std::cout << "Pop: no= " << _nodeCount << ", addr= " << _top 
    << " data= " << _top->GetData() << std::endl;
#endif
  delete _top;
  _top = prev;
  _nodeCount--;

  return ret;
}

template <typename T>
bool DynamicStack<T>::IsEmpty(void)
{
  if (_top == NULL)
    return true;
  else
    return false;
}

template <typename T>
void DynamicStack<T>::PrintStack(void)
{
  if (IsEmpty())
  {
    std::cout << "PrintStack: empty" << std::endl;
    return;
  }

  SingleNode<T> * runner = _top;
  size_t i = _nodeCount;
  while (runner != NULL) {
    std::cout << "PrintStack: no= " << i << ", addr= " << runner
      << " data= " << runner->GetData() << std::endl;
    runner = runner->GetPrev();
    --i;
  }
}

template <typename T>
void DynamicStack<T>::ClearStack(void)
{
  SingleNode<T> * runner = _top;
  SingleNode<T> * prev;
  
  while (runner != NULL) {
    prev = runner->GetPrev();
#ifdef DEBUG_ENABLED
  std::cout << "ClearStack: no= " << _nodeCount << ", del addr= " << runner 
    << " data= " << runner->GetData() << std::endl;
#endif
    delete runner;
    runner = prev;
    _nodeCount--;
  }

  _top = NULL;
  _nodeCount = 0;
}

#endif // end of STACK_H
