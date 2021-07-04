/*           
 * Title        :   Data Structure Example
 * Filename     :   circulardoublylist.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 3. 
 * Circular Doubly Linked List by cpp
 */

#ifndef CIRCULAR_DOUBLY_LIST_H
#define CIRCULAR_DOUBLY_LIST_H

#define DEBUG_ENABLED

#include <iostream>

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
    void DeleteNode(T data);
    void PrintListForward(void);
    void PrintListBackward(void);
    void ClearList(void);

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
  _head = NULL; 
  _tail = NULL;
  _nodeCount = 0; 
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

#endif // end of CIRCULAR_DOUBLY_LIST_H
