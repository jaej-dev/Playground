//*****************************************************************
//  LinkedList.cpp
//  HashTable
//
//  Created by Karlina Beringer on June 16, 2014.
//
//  This m_header file contains the Linked List class declaration.
//  Hash Table array elements consist of Linked List objects.
//
//  Modified by jaej.dev@google.com
//  20151013: fixed memory leak
//            if you want to see memory leaking behavior, 
//            build origin and then run "valgrind --leak-check=yes ./run"
//*****************************************************************

#include "LinkedList.h"

// Constructs the empty linked list object.
// Creates the m_head node and sets m_listLength to zero.
LinkedList::LinkedList()
{
    m_head = new Item;
    m_head -> next = NULL;
    m_listLength = 0;

#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << ", m_head addr = " << m_head
			<< std::endl;
#endif		
}

// Inserts an item at the end of the list.
void LinkedList::listInsertItem( Item * newItem )
{
    if (!m_head -> next)
    {
        m_head -> next = newItem;
        m_listLength++;
#ifdef ENABLE_DEBUG
				std::cout << __func__ << ": list length  = " << m_listLength 
					<< ", addr = " << newItem << std::endl;
#endif		
        return;
    }
    Item * p = m_head;
    Item * q = m_head;
    while (q)
    {
        p = q;
        q = p -> next;
    }
    p -> next = newItem;
    newItem -> next = NULL;
    m_listLength++;

#ifdef ENABLE_DEBUG
		std::cout << __func__ << ": list length  = " << m_listLength 
			<< ", addr = " << newItem << std::endl;
#endif		
}

// Removes an item from the list by item key.
// Returns true if the operation is successful.
bool LinkedList::listRemoveItem( string itemKey )
{
    if (!m_head -> next) return false;
    Item * p = m_head;
    Item * q = m_head;
    while (q)
    {
        if (q -> key == itemKey)
        {
            p -> next = q -> next;
            delete q;
            m_listLength--;
            return true;
        }
        p = q;
        q = p -> next;
    }

#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": call count = " << ++i << " list length  = " 
			<< m_listLength << std::endl;
#endif

    return false;
}

// Searches for an item by its key.
// Returns a reference to first match.
// Returns a NULL pointer if no match is found.
Item * LinkedList::listGetItem( string itemKey )
{
    Item * p = m_head;
    Item * q = m_head;
    while (q)
    {
        p = q;
        if ((p != m_head) && (p -> key == itemKey))
            return p;
        q = p -> next;
    }
    return NULL;
}

// Displays list contents to the console window.
void LinkedList::listPrintList()
{
    if (m_listLength == 0)
    {
        cout << "\n{ }\n";
        return;
    }
    Item * p = m_head;
    Item * q = m_head;
    cout << "\n{ ";
    while (q)
    {
        p = q;
        if (p != m_head)
        {
            cout << p -> key;
            if (p -> next) cout << ", ";
            else cout << " ";
        }
        q = p -> next;
    }
    cout << "}\n";
}

// Returns the m_listLength of the list.
int LinkedList::listGetLength()
{
    return m_listLength;
}

// De-allocates list memory when the program terminates.
LinkedList::~LinkedList()
{
		Item * p = m_head->next;
		Item * q = m_head->next;
		
		while (q)
		{
			p = q;
			q = p->next;
#ifdef ENABLE_DEBUG
			static int i = 0;
			std::cout << __func__ << ": deleted count = " << ++i << ", addr = " 
				<< p << ", key = " << p -> key << endl;
#endif
			delete p;
		}
		if (m_head) delete m_head;

#if 0
		/* memory leak code */
    Item * p = m_head;
    Item * q = m_head;
    while (q)
    {
        p = q;
        q = p -> next;
#ifdef ENABLE_DEBUG
        if (q)
				{ 
					static int i = 0;
					std::cout << __func__ << ": deleted count = " << ++i << ", addr = " 
						<< p << endl;
					delete p;
				}
#else
        if (q) delete p;
#endif
    }
#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << std::endl;
#endif		
#endif
}

//*****************************************************************
// End of File
//*****************************************************************
