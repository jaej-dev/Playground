/*           
 * Title        :   Linked Lists Example
 * Filename     :   singlylist.h
 * Author       :   jaej.dev@gmail.com
 * Description  :
 * This source code is based from /cci/Ch00_Reference/r01_singly_linked_list.
 * 
 * Question 13: sum two list
 * You have two numbers represented by a linked list, where each node contains 
 * a single digit. The digits are stored in reverse order, such that the 1 's 
 * digit is at the head  of the list. Write a function that adds the two 
 * numbers and returns the sum as a linked list.
 * 
 * EXAMPLE
 * Input: (7-> 1 -> 6) + (5 -> 9 -> 2).That is, 617 + 295.
 * Output: 2 -> 1 -> 9.That is, 912.
 *
 * FOLLOW UP
 * Suppose the digits are stored in forward order. Repeat the above problem.
 * EXAMPLE
 * Input: (6 -> 1 -> 7) + (2 -> 9 -> 5).That is, 617 + 295.
 * Output: 9 -> 1 -> 2.That is, 912.
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
class SingleLinkedList
{
  public:
    SingleLinkedList() : _head(NULL), _nodeCount(0) {}
    SingleLinkedList(T data) : _head(new SingleNode<T>(data)) {}
    ~SingleLinkedList() { _head = NULL; _nodeCount = 0; }
    SingleNode<T> * GetHead(void) { return _head; }
    void SetHead(SingleNode<T> * head) { _head = head; }
    void AppendNode(T); 
    void DeleteNode(T);
    void DeleteNodeByNode(SingleNode<T> *);
    void RemoveDuplicateNode(void);
    T FindKthToLast(int);
    SingleNode<T> * GetMiddleNode(void);
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

/* SingleLinkedList implementation */
template <typename T>
void SingleLinkedList<T>::AppendNode(T data)
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
  std::cout << "Append: list= " << this << ", node= " << _nodeCount 
    << ", addr= " << node << " data= " << node->GetData() << std::endl;
#endif
}

template <typename T>
void SingleLinkedList<T>::DeleteNode(T data)
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
void SingleLinkedList<T>::DeleteNodeByNode(SingleNode<T>* n)
{
  SingleNode<T> * next = n->GetNext();
  if (n == NULL || next == NULL)
  {
    std::cout << "DeleteNodeByNode: target node is wrong" << std::endl;
    return;
  }
  // copy from next node to target node, and then delete next node.
  n->SetNext(next->GetNext());
  n->SetData(next->GetData());
  delete next;
}

template <typename T>
void SingleLinkedList<T>::RemoveDuplicateNode(void)
{
  SingleNode<T> * curr = _head;

  while (curr != NULL) {
    // check all subsequent node for duplicates
    SingleNode<T> * runner = curr->GetNext();
    SingleNode<T> * prev = curr;
    while (runner != NULL) {
      if (curr->GetData() == runner->GetData())
      {
#ifdef DEBUG_ENABLED
        std::cout << "RemoveDuplicateNode: addr= " << runner << ", data= "
          << runner->GetData() << std::endl;
#endif
        SingleNode<T> * temp = runner;  // target remove node
        runner = runner->GetNext();
        prev->SetNext(runner);  // to jump removed node
        delete temp;
      }
      else
      {
        prev = runner;
        runner = runner->GetNext();
      }
    }
    curr = curr->GetNext();
  }
}

template <typename T>
T SingleLinkedList<T>::FindKthToLast(int k)
{
  SingleNode<T> * p1 = _head;
  SingleNode<T> * p2 = _head;

  if (k < 1)
  {
    std::cout << "FindKthToLast: kTh is too small" << std::endl;
    return (T)-1;
  }

  if (p1 == NULL)
  {
    std::cout << "FindKthToLast: empty" << std::endl;
    return (T)-1;
  }

  // move p2 forward k nodes into the list
  for (int i = 0; i < k-1; ++i)
  {
    if (p2 == NULL)
    {
      std::cout << "FindKthToLast: kTh is too big" << std::endl;
      return (T)-1;
    }
    p2 = p2->GetNext();
  }
  
  // when p2 hit the end, p1 will be at the right element
  while ((p2 = p2->GetNext()) != NULL)
    p1 = p1->GetNext();

  return (T)p1->GetData(); 
}

template <typename T>
SingleNode<T> * SingleLinkedList<T>::GetMiddleNode(void)
{
  SingleNode<T> * temp = _head;

  if (temp == NULL)
    return NULL;
  
  for (int i = 0; i < (_nodeCount-1)/2; ++i)
    temp = temp->GetNext();

  return temp;
}

template <typename T>
void SingleLinkedList<T>::PrintList(void)
{
  SingleNode<T> * temp = _head;
  if (temp == NULL)
  {
    std::cout << "PrintList: list= " << this << ", empty" << std::endl;
    return;
  }

  size_t i = 1;
  while(temp != NULL) {
    std::cout << "PrintList: list= " << this << ", node= " << i 
      << ", addr= " << temp << ", data= " << temp->GetData() << std::endl;
    i++;
    temp = temp->GetNext();
  }
}

template <typename T>
void SingleLinkedList<T>::ClearList(void)
{
  SingleNode<T> * temp = _head;
  SingleNode<T> * prev = NULL;

  if (temp == NULL)
  {
    std::cout << "ClearList: list= " << this << ", empty" << std::endl;
    return;
  }
  
  while(temp != NULL)
  {
    prev = temp;
    temp = temp->GetNext();
#ifdef DEBUG_ENABLED
    std::cout << "ClearList: list= " << this << ", node= " << _nodeCount 
      << ", addr= " << prev << ", data= " << prev->GetData() << std::endl;
#endif      
    _nodeCount--;
    delete prev;
  }
  //delete _head; // bug fixed: invalid delete, it would be delete by destroyer.
  _head = NULL;
}

#endif // end of SINGLY_LIST_H
