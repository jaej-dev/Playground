/*           
 * Title        :   Chapter2 Linked Lists
 * Filename     :   main.cc
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

#ifdef __cplusplus
#endif

#include <iostream>
#include <string>
#include "singlylist.h"

using namespace std;

SingleLinkedList<int> * SumTwoListByBackward(SingleLinkedList<int> * l1, 
    SingleLinkedList<int> * l2)
{
  SingleLinkedList<int> * result = new SingleLinkedList<int>(); 
  SingleNode<int> * x = l1->GetHead();
  SingleNode<int> * y = l2->GetHead();
  int carry = 0;
  int sum = 0;

  while (x != NULL && y != NULL) {
    // least digit
    sum = x->GetData() + y->GetData() + carry;
    carry = sum / 10;
    sum = sum % 10;
    result->AppendNode(sum);
    x = x->GetNext();
    y = y->GetNext();
  }
  if (x != NULL && y == NULL)
  {
    sum = x->GetData() + carry;
    carry = sum / 10;
    sum = sum % 10;
    result->AppendNode(sum);
    x = x->GetNext();
    while (x != NULL) {
      sum = x->GetData() + carry;
      carry = sum / 10;
      sum = sum % 10;
      result->AppendNode(x->GetData());
      x = x->GetNext();
    }
  }
  else if (x == NULL && y != NULL)
  {
    sum = y->GetData() + carry;
    carry = sum / 10;
    sum = sum % 10;
    result->AppendNode(y->GetData() + carry);
    y = y->GetNext();
    while (y != NULL) {
      sum = x->GetData() + carry;
      carry = sum / 10;
      sum = sum % 10;
      result->AppendNode(sum);
      y = y->GetNext();
    }
  }

  if (carry)
    result->AppendNode(carry);  // most digit

  return result;
}

/*
Reserve algorithm:
    init   =      (HEAD)1->    2->    3->    NULL
    step 1 =  NULL    <-1      2->    3->    NULL
    step 2 =  NULL    <-1    <-2      3->    NULL
    step 3 =  NULL    <-1    <-2    <-3
    final  =  NULL    <-1    <-2    <-3(HEAD)
*/
void ReverseSingleLinkedList(SingleLinkedList<int> * l)
{
  // init
  SingleNode<int> * curr = l->GetHead();
  SingleNode<int> * prev = NULL;
  SingleNode<int> * next = NULL;

  if (curr == NULL)
  {
    cout << "ReverseSingleLinkedList: empty" << endl;
    return;
  }
  // step 1~N
  while (curr != NULL) {
    next = curr->GetNext();
    curr->SetNext(prev);  // change pointer to previous node
    prev = curr;
    curr = next;
  }
  // final
  l->SetHead(prev);
}

SingleLinkedList<int> * SumTwoListByForward(SingleLinkedList<int> * l1, 
    SingleLinkedList<int> * l2)
{
  ReverseSingleLinkedList(l1);
  ReverseSingleLinkedList(l2);
  
  SingleLinkedList<int> * result = new SingleLinkedList<int>(); 
  SingleNode<int> * x = l1->GetHead();
  SingleNode<int> * y = l2->GetHead();
  int carry = 0;
  int sum = 0;

  while (x != NULL && y != NULL) {
    // least digit
    sum = x->GetData() + y->GetData() + carry;
    carry = sum / 10;
    sum = sum % 10;
    result->AppendNode(sum);
    x = x->GetNext();
    y = y->GetNext();
  }
  if (x != NULL && y == NULL)
  {
    sum = x->GetData() + carry;
    carry = sum / 10;
    sum = sum % 10;
    result->AppendNode(sum);
    x = x->GetNext();
    while (x != NULL) {
      sum = x->GetData() + carry;
      carry = sum / 10;
      sum = sum % 10;
      result->AppendNode(x->GetData());
      x = x->GetNext();
    }
  }
  else if (x == NULL && y != NULL)
  {
    sum = y->GetData() + carry;
    carry = sum / 10;
    sum = sum % 10;
    result->AppendNode(y->GetData() + carry);
    y = y->GetNext();
    while (y != NULL) {
      sum = x->GetData() + carry;
      carry = sum / 10;
      sum = sum % 10;
      result->AppendNode(sum);
      y = y->GetNext();
    }
  }

  if (carry)
    result->AppendNode(carry);  // most digit

  ReverseSingleLinkedList(result);

  return result;
}

int CalcDivisor(int n)
{
  int div = 1000000000;
  while (n/div < 1)
    div = div/10;
  return div;
}

int main (void)
{
  int n1 = 9769;
  int n2 = 839;
  SingleLinkedList<int> * l1 = new SingleLinkedList<int>;
  SingleLinkedList<int> * l2 = new SingleLinkedList<int>;

  cout << "Step 1: Given number 1 = " << n1 << ", number 2= " << n2 
    << ", backword sum = " << 9679+938 << ", forword sum = " << n1+n2 << endl;

  cout << "Step 3: Add given number into the list 1" << endl;
  for (int div = CalcDivisor(n1); div > 0; div = div/10)
  {
    l1->AppendNode(n1/div);
    n1 = n1 % div;
  }

  cout << "Step 4: Add given number into the list 2" << endl;
  for (int div = CalcDivisor(n2); div > 0; div = div/10)
  {
    l2->AppendNode(n2/div);
    n2 = n2 % div;
  }

  cout << "Step 4: Sum two list by backward" << endl;
  SingleLinkedList<int> * result1 = SumTwoListByBackward(l1, l2);

  cout << "Step 5: Backward result" << endl;
  result1->PrintList();

  cout << "Step 6: Sum two list by forward" << endl;
  SingleLinkedList<int> * result2 = SumTwoListByForward(l1, l2);

  cout << "Step 7: Forward result" << endl;
  result2->PrintList();

  cout << "Step 8: Delete all nodes" << endl;
  l1->ClearList();
  l2->ClearList();
  result1->ClearList();
  result2->ClearList();

  cout << "Step 9: Print all nodes" << endl;
  l1->PrintList();
  l2->PrintList();
  result1->PrintList();
  result2->PrintList();

  delete l1;
  delete l2;
  delete result1;
  delete result2;

  return 0;
}
