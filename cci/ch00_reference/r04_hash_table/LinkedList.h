//*****************************************************************
//  LinkedList.h
//  HashTable
//
//  Created by Karlina Beringer on June 16, 2014.
//
//  This header file contains the Linked List class declaration.
//  Hash Table array elements consist of Linked List objects.
//*****************************************************************

#ifndef LinkedList_h
#define LinkedList_h

#include <iostream>
#include <string>
using namespace std;

#define ENABLE_DEBUG

//*****************************************************************
// List items are keys with pointers to the next item.
//*****************************************************************
typedef struct _Item
{
    string key;
    struct _Item * next;
} Item;

//*****************************************************************
// Linked lists store a variable number of items.
//*****************************************************************
class LinkedList
{
private:
    // Head is a reference to a list of data nodes.
    Item * m_head;
    
    // Length is the number of data nodes.
    int m_listLength;
    
public:
    // Constructs the empty linked list object.
    // Creates the head node and sets length to zero.
    LinkedList();
    
    // Inserts an item at the end of the list.
    void listInsertItem( Item * newItem );
    
    // Removes an item from the list by item key.
    // Returns true if the operation is successful.
    bool listRemoveItem( string itemKey );
    
    // Searches for an item by its key.
    // Returns a reference to first match.
    // Returns a NULL pointer if no match is found.
    Item * listGetItem( string itemKey );
    
    // Displays list contents to the console window.
    void listPrintList();
    
    // Returns the length of the list.
    int listGetLength();
    
    // De-allocates list memory when the program terminates.
    ~LinkedList();
};

#endif

//*****************************************************************
// End of File
//*****************************************************************
