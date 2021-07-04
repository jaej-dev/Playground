/*           
 * Title        :   Data Structure Example
 * Filename     :   doublylist.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 2. 
 * Doubly Linked List by cpp
 */

#ifndef DOUBLY_LIST_H
#define DOUBLY_LIST_H

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
class DoublyLinkedList
{
  public:
    DoublyLinkedList();
    DoublyLinkedList(T data);
    ~DoublyLinkedList();
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
/* DoublyLinkedList implementation */
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
{
  _head = NULL;
  _tail = NULL;
   _nodeCount = 0;
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(T data)
{
  DoubleNode<T> * node = new DoubleNode<T>(data);
  _head = node;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{ 
  _head = NULL; 
  _tail = NULL;
  _nodeCount = 0; 
}

/*
 * HEAD -> 3 <-> 2 <-> 1 <- TAIL
 */
template <typename T>
void DoublyLinkedList<T>::AppendNodeFront(T data)
{
  DoubleNode<T> * node = new DoubleNode<T>(data);

  if ( _head == NULL)
  {
    _head = node;
    _tail = node;
  }
  else
  {
    _head->SetPrev(node);
    node->SetNext(_head);
    _head = node;
  }
  
 _nodeCount++;
#ifdef DEBUG_ENABLED
  std::cout << "AppendNodeFront: no= " << _nodeCount << ", tail= " << _tail 
    << ", node= " << node  << ", data= " << node->GetData() 
    << ", head= " << _head << std::endl;
#endif
}

/*
 * HEAD -> 1 <-> 2 <-> 3 <- TAIL
 */
template <typename T>
void DoublyLinkedList<T>::AppendNodeBack(T data)
{
  DoubleNode<T> * node = new DoubleNode<T>(data);

  if ( _tail == NULL)
  {
    _head = node;
    _tail = node;
  }
  else
  {
    _tail->SetNext(node);
    node->SetPrev(_tail);
    _tail = node;
  }
  
 _nodeCount++;
#ifdef DEBUG_ENABLED
  std::cout << "AppendNodeBack: no= " << _nodeCount << ", tail= " << _tail 
    << ", node= " << node  << ", data= " << node->GetData()
    << ", head= " << _head << std::endl;
#endif
}

template <typename T>
void DoublyLinkedList<T>::DeleteNode(T data)
{
  DoubleNode<T> * runner = _head;
  if (runner == NULL)
  {
    std::cout << "DeleteNode: empty" << std::endl;
    return;
  }

  DoubleNode<T> * prev = runner;
  while (runner != NULL) {
    if (runner->GetData() == data)
    {
#ifdef DEBUG_ENABLED
      std::cout << "DeleteNode: no = " << _nodeCount << ", addr= " 
      << runner << ", data= " << runner->GetData() << std::endl;
#endif      
      if (runner == _head)
      {
        _head = _head->GetNext();
        _head->SetPrev(NULL);
      }
      else if (runner == _tail)
      {
        _tail = prev;
        _tail->SetNext(NULL);
      }
      else
      {
        DoubleNode<T> * next = runner->GetNext();
        prev->SetNext(next);
        next->SetPrev(prev);
      }
      delete runner;
      runner = NULL;
      _nodeCount--;
    }
    else
    {
      prev = runner;
      runner = runner->GetNext();
    }
  }
}

/*
 * Tail to Head
 */
template <typename T>
void DoublyLinkedList<T>::PrintListForward(void)
{
  DoubleNode<T> * temp = _head;
  if (temp == NULL)
  {
    std::cout << "PrintListForward: empty" << std::endl;
    return;
  }

  size_t i = 0;
  while(temp != NULL) {
    std::cout << "PrintListForward: no= " << ++i << ", addr= " << temp
      << ", data= " << temp->GetData() << std::endl;
    temp = temp->GetNext();
  }
}

template <typename T>
void DoublyLinkedList<T>::PrintListBackward(void)
{
  DoubleNode<T> * temp = _tail;
  if (temp == NULL)
  {
    std::cout << "PrintListBackward: empty" << std::endl;
    return;
  }

  size_t i = 0;
  while(temp != NULL) {
    std::cout << "PrintListBackward: no= " << ++i << ", addr= " << temp
      << ", data= " << temp->GetData() << std::endl;
    temp = temp->GetPrev();
  }
}

template <typename T>
void DoublyLinkedList<T>::ClearList(void)
{
  DoubleNode<T> * runner = _head;
  DoubleNode<T> * prev = NULL;

  if (runner == NULL)
  {
    std::cout << "ClearList: empty" << std::endl;
    return;
  }
  
  while(runner != NULL)
  {
    prev = runner;
    runner = runner->GetNext();
#ifdef DEBUG_ENABLED
    std::cout << "ClearList: node= " << _nodeCount << ", addr= " << prev << 
      ", data= " << prev->GetData() << std::endl;
#endif      
    _nodeCount--;
    delete prev;
  }
  //delete _head; // bug fixed: invalid delete, it would be delete by destroyer.
  _head = NULL;
  _tail = NULL;
}

#endif // end of DOUBLY_LIST_H
