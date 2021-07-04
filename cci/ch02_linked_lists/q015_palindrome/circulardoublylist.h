/*           
 * Title        :   Linked Lists Example
 * Filename     :   circularduoblylist.h
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

#ifndef CIRCULAR_DOUBLY_LIST_H
#define CIRCULAR_DOUBLY_LIST_H

#define DEBUG_ENABLED

#include <iostream>
#include "stack.h"

template <typename T>
class DoubleNode
{
  public: 
    DoubleNode() : _data(0), _prev(NULL), _next(NULL) {}
    DoubleNode(T data) : _data(data), _prev(NULL), _next(NULL) {}
    ~DoubleNode() { _data = 0; _prev = NULL, _next = NULL; }
    T GetData();
    void SetData(T);
    DoubleNode<T> * GetNext();
    void SetNext(DoubleNode<T> *);
    DoubleNode<T> * GetPrev();
    void SetPrev(DoubleNode<T> *);

  private:
    T _data;
    DoubleNode<T> * _prev;
    DoubleNode<T> * _next;
};

template <typename T>
class CircularDoublyLinkedList
{
  public:
    CircularDoublyLinkedList();
    CircularDoublyLinkedList(T data);
    ~CircularDoublyLinkedList();
    void AppendNodeFront(T data);
    void AppendNodeBack(T data);
    int Size(void);
    void DeleteNode(T data);
    void PrintListForward(void);
    void PrintListBackward(void);
    void ClearList(void);
    DoubleNode<T> * FindBeginningLoop(void);
    bool CheckPalindrome(void);

  private:
    DoubleNode<T> * _head;
    DoubleNode<T> * _tail;
    size_t _nodeCount;
};

/* DoubleNode implementation */
template <typename T>
T DoubleNode<T>::GetData(void)
{
  return _data;
}

template <typename T>
void DoubleNode<T>::SetData(T data)
{
  _data = data;
}

template <typename T>
DoubleNode<T> * DoubleNode<T>::GetNext(void)
{
  return _next;
}

template <typename T>
void DoubleNode<T>::SetNext(DoubleNode<T>* next)
{
  _next = next;
}

template <typename T>
DoubleNode<T> * DoubleNode<T>::GetPrev(void)
{
  return _prev;
}

template <typename T>
void DoubleNode<T>::SetPrev(DoubleNode<T>* prev)
{
  _prev = prev;
}
/* CircularDoublyLinkedList implementation */
template <typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList()
{
  _head = NULL;
  _tail = NULL;
   _nodeCount = 0;
}

template <typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList(T data)
{
  DoubleNode<T> * node = new DoubleNode<T>(data);
  _head = node;
}

template <typename T>
CircularDoublyLinkedList<T>::~CircularDoublyLinkedList()
{
  CircularDoublyLinkedList<T>::ClearList();
}

/*
 *         +----------->
 * HEAD -> 3 <-> 2 <-> 1 <- TAIL
 *         <-----------+
 */
template <typename T>
void CircularDoublyLinkedList<T>::AppendNodeFront(T data)
{
  DoubleNode<T> * node = new DoubleNode<T>(data);

  if ( _head == _tail && _head == NULL)
  {
    _head = _tail = node;
    _head->SetNext(_head);
    _head->SetPrev(_head);
  }
  else
  {
    node->SetNext(_head);
    _head->SetPrev(node);
    _head = node;
    _head->SetPrev(_tail);
    _tail->SetNext(_head);
  }
  
 _nodeCount++;
#ifdef DEBUG_ENABLED
  std::cout << "AppendNodeFront: no= " << _nodeCount << ", node= " << node
    << ", data= " << node->GetData() << ", head= " << _head
    << ", tail= " << _tail << std::endl;
#endif
}

/*
 *         +----------->
 * HEAD -> 1 <-> 2 <-> 3 <- TAIL
 *         <-----------+
 */
template <typename T>
void CircularDoublyLinkedList<T>::AppendNodeBack(T data)
{
  DoubleNode<T> * node = new DoubleNode<T>(data);

  if ( _tail == _head && _tail == NULL)
  {
    _tail = _head = node;
    _tail->SetNext(_tail);
    _tail->SetPrev(_tail);
  }
  else
  {
    _tail->SetNext(node);
    node->SetPrev(_tail);
    _tail = node;
    _head->SetPrev(_tail);
    _tail->SetNext(_head);
  }
  
  _nodeCount++;
#ifdef DEBUG_ENABLED
  std::cout << "AppendNodeBack: no= " << _nodeCount << ", node= " << node  
    << ", data= " << node->GetData() << ", head= " << _head 
    << ", tail= " << _tail << std::endl;
#endif
}

template <typename T>
void CircularDoublyLinkedList<T>::DeleteNode(T data)
{
  DoubleNode<T> * runner = _head;
  if (runner == NULL)
  {
    std::cout << "DeleteNode: empty" << std::endl;
    return;
  }

  // check first node
  if (runner->GetData() == data)
  {
    DoubleNode<T> * next = runner->GetNext();
    if (next == _head)
    {
#ifdef DEBUG_ENABLED
      std::cout << "DeleteNode: no= " << _nodeCount << ", addr= " 
      << runner << ", data= " << runner->GetData() 
      << ", head= " << _head << ", tail= "<< _tail << std::endl;
#endif
      delete runner;
      _head = NULL;
      _tail = NULL;
      _nodeCount--;
      return;
    }
    else
    {
      next->SetPrev(_tail);
      _head = next;
      _tail->SetNext(_head);
#ifdef DEBUG_ENABLED
      std::cout << "DeleteNode: no= " << _nodeCount << ", addr= " 
        << runner << ", data= " << runner->GetData() 
        << ", head= " << _head << ", tail= "<< _tail << std::endl;
#endif      
      delete runner;
      _nodeCount--;
      return;
    }
  }
  else
  {
    runner = runner->GetNext();
  }

  // check other nodes
  DoubleNode<T> * prev = runner;
  while (runner != _head) {
    if (runner->GetData() == data)
    {
      if (runner == _tail)
      {
        _tail = prev;
        _tail->SetNext(_head);
      }
      else
      {
        DoubleNode<T> * next = runner->GetNext();
        prev->SetNext(next);
        next->SetPrev(prev);
      }
#ifdef DEBUG_ENABLED
      std::cout << "DeleteNode: no= " << _nodeCount << ", addr= " 
        << runner << ", data= " << runner->GetData() 
        << ", head= " << _head << ", tail= "<< _tail << std::endl;
#endif      
      delete runner;
      runner = NULL;
      _nodeCount--;
      return;
    }
    else
    {
      prev = runner;
      runner = runner->GetNext();
    }
  }
  std::cout << "DeleteNode: can not found " << data << std::endl;
}

/*
 * the number of elements in the container
 */
template <typename T>
int CircularDoublyLinkedList<T>::Size(void)
{
  return _nodeCount;
}
/*
 * Tail to Head
 */
template <typename T>
void CircularDoublyLinkedList<T>::PrintListForward(void)
{
  DoubleNode<T> * temp = _head;
  if (temp == NULL)
  {
    std::cout << "PrintListForward: empty" << std::endl;
    return;
  }

  for (size_t i = 1; i < _nodeCount+1; ++i)
  {
    std::cout << "PrintListForward: no= " << i << ", addr= " << temp
      << ", data= " << temp->GetData() << ", head= " << _head 
      << ", tail= " << _tail << std::endl;
    temp = temp->GetNext();
  }
}

template <typename T>
void CircularDoublyLinkedList<T>::PrintListBackward(void)
{
  DoubleNode<T> * temp = _tail;
  if (temp == NULL)
  {
    std::cout << "PrintListBackward: empty" << std::endl;
    return;
  }

  for (size_t i = 1; i < _nodeCount+1; ++i)
  {
    std::cout << "PrintListBackward: no= " << i << ", addr= " << temp
      << ", data= " << temp->GetData() << ", head= " << _head 
      << ", tail= " << _tail << std::endl;
    temp = temp->GetPrev();
  }
}

template <typename T>
void CircularDoublyLinkedList<T>::ClearList(void)
{
  DoubleNode<T> * runner = _head;
  DoubleNode<T> * prev = NULL;

  if (runner == NULL)
  {
    std::cout << "ClearList: empty" << std::endl;
    return;
  }

  while (_nodeCount) {
    prev = runner;
#ifdef DEBUG_ENABLED
    std::cout << "ClearList: no= " << _nodeCount << ", addr= " << runner
      << ", data= " << runner->GetData() << ", head= " << _head 
      << ", tail= " << _tail << std::endl;
#endif
    runner = runner->GetNext();
    delete prev;
    _nodeCount--;
  }
  _head = NULL;
  _tail = NULL;
}
template <typename T>
DoubleNode<T> * CircularDoublyLinkedList<T>::FindBeginningLoop(void)
{
  DoubleNode<T> * fast = _head;
  DoubleNode<T> * slow = _head;

  // find collision spot
  while (fast != NULL && fast->GetNext() != NULL) {
    DoubleNode<T> * next = fast->GetNext();
    slow = slow->GetNext();
    fast = next->GetNext();
#ifdef DEBUG_ENABLED
    std::cout << "FindBeginningLoop: slow= " << slow 
      << ", fast= " << fast << std::endl;
#endif    
    if (slow == fast)
    {
#ifdef DEBUG_ENABLED
    std::cout << "FindBeginningLoop: found collision node "
      << slow << std::endl;
#endif    
      if (slow == _head )
      {
#ifdef DEBUG_ENABLED
        std::cout << "FindBeginningLoop: it's a head" << std::endl;
#endif    
        return NULL;
      }
      break;
    }
  }

  // find node of beginning loop
  slow = _head;
  while (slow != fast)
  {
    slow = slow->GetNext();
    fast = fast->GetNext();
  }

  return fast;
}

/*
 * Check palindrome
 */
template <typename T>
bool CircularDoublyLinkedList<T>::CheckPalindrome(void)
{
  if (_nodeCount % 2 == 0)
  {
    std::cout << "CheckPalindrome: the number of elements is even number (" 
      << _nodeCount << ")" << std::endl;
    return false;
  }

  // method 1: using head and tail
  DoubleNode<T> * front = _head;
  DoubleNode<T> * back = _tail;
  if (front == NULL || back == NULL)
  {
    std::cout << "CheckPalindrome: empty" << std::endl;
    return false;
  }

  do {
    if (front->GetData() != back->GetData())
    {
#ifdef DEBUG_ENABLED
      std::cout << "CheckPalindrome: front= " << front->GetData() 
        << ", back= " << back->GetData() << std::endl;
#endif      
      return false;
    }
    front = front->GetNext();
    back = back->GetPrev();
  } while (front != back);

#ifdef DEBUG_ENABLED
  std::cout << "CheckPalindrome: front addr= " << front << ", back addr= "
    << back << std::endl;
#endif

  return true;
}
#endif // end of CIRCULAR_DOUBLY_LIST_H
