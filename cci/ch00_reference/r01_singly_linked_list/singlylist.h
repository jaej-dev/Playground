/*           
 * Title        :   Data Structure Example
 * Filename     :   singlylist.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 1. 
 * Singly Linked List by cpp
 */

#ifndef SINGLY_LIST_H
#define SINGLY_LIST_H

#define DEBUG_ENABLED

#include <iostream>

template <typename T>
class SingleNode
{
  public: 
    SingleNode() : _data(0), _next(NULL) {}
    SingleNode(T data) : _data(data), _next(NULL) {}
    ~SingleNode() { _data = 0; _next = NULL; }
    T GetData();
    void SetData(T);
    SingleNode<T> * GetNext();
    void SetNext(SingleNode<T> *);

  private:
    T _data;
    SingleNode<T> * _next;
};

template <typename T>
class SinglyLinkedList
{
  public:
    SinglyLinkedList() : _head(NULL), _nodeCount(0) {}
    SinglyLinkedList(T data) : _head(new SingleNode<T>(data)) {}
    ~SinglyLinkedList() { _head = NULL; _nodeCount = 0; }
    void AppendNode(T);
    void DeleteNode(T);
    void PrintList(void);
    void ClearList(void);

  private:
    SingleNode<T> * _head;
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
SingleNode<T> * SingleNode<T>::GetNext(void)
{
  return _next;
}

template <typename T>
void SingleNode<T>::SetNext(SingleNode<T>* next)
{
  _next = next;
}

/* SinglyLinkedList implementation */
template <typename T>
void SinglyLinkedList<T>::AppendNode(T data)
{
  // create new node
  SingleNode<T> * node = new SingleNode<T>();
  node->SetData(data);
  node->SetNext(NULL);

  // append new node
  SingleNode<T> * temp = _head;
  if (temp != NULL)
  {
    while(temp->GetNext() != NULL) {
      temp = temp->GetNext();
    }
    temp->SetNext(node);
  }
  else
  {
    _head = node;
  }
  _nodeCount++;
#ifdef DEBUG_ENABLED
  std::cout << "Append: node= " << _nodeCount << ", addr= " << node 
    << " data= " << node->GetData() << std::endl;
#endif
}

template <typename T>
void SinglyLinkedList<T>::DeleteNode(T data)
{
  SingleNode<T> * runner = _head;
  if (runner == NULL)
  {
    std::cout << "DeleteNode: empty" << std::endl;
    return;
  }

  SingleNode<T> * prev = runner; // bug fix: to delete head
  while (runner != NULL) {
    if (runner->GetData() == data)
    {
#ifdef DEBUG_ENABLED
      std::cout << "DeleteNode: node = " << _nodeCount << ", addr= " 
      << runner << ", data= " << runner->GetData() << std::endl;
#endif      
      if (prev == _head)
        _head = prev->GetNext();  // bug fix: to delete head
      else
        prev->SetNext(runner->GetNext());
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

template <typename T>
void SinglyLinkedList<T>::PrintList(void)
{
  SingleNode<T> * temp = _head;
  if (temp == NULL)
  {
    std::cout << "PrintList: empty" << std::endl;
    return;
  }

#ifdef DEBUG_ENABLED
  size_t i = 1;
#endif
  while(temp != NULL) {
#ifdef DEBUG_ENABLED
    std::cout << "PrintList: node= " << i << ", addr= " << temp
      << ", data= " << temp->GetData() << std::endl;
    i++;
#endif      
    temp = temp->GetNext();
  }
}

template <typename T>
void SinglyLinkedList<T>::ClearList(void)
{
  SingleNode<T> * temp = _head;
  SingleNode<T> * prev = NULL;

  if (temp == NULL)
  {
    std::cout << "ClearList: empty" << std::endl;
    return;
  }
  
  while(temp != NULL)
  {
    prev = temp;
    temp = temp->GetNext();
#ifdef DEBUG_ENABLED
    std::cout << "ClearList: node= " << _nodeCount << ", addr= " << prev << 
      ", data= " << prev->GetData() << std::endl;
#endif      
    _nodeCount--;
    delete prev;
  }
  //delete _head; // bug fixed: invalid delete, it would be delete by destroyer.
  _head = NULL;
}

#endif // end of SINGLY_LIST_H
