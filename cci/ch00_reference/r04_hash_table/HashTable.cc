//*****************************************************************
//  HashTable.cpp
//  HashTable
//
//  Created by Kar Beringer on June 18, 2014.
//
//  This header file contains the Hash Table class definition.
//  Hash Table m_array elements consist of Linked List objects.
//*****************************************************************

#include <cassert>
#include "HashTable.h"

// Constructs the empty Hash Table object.
// Array m_hashLength is set to 13 by default.
HashTable::HashTable( int tableLength )
{
    if (tableLength <= 0) tableLength = 13;
    m_array = new LinkedList[ tableLength ];
    m_hashLength = tableLength;
#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << std::endl;
#endif		
}

// Returns an m_array location for a given item key.
int HashTable::hash( string itemKey )
{
    int value = 0;
    for ( int i = 0; i < (int)itemKey.length(); i++ )
        value += itemKey[i];
#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << ", itemKey = " << itemKey
			<< ", value = " << value << ", itemKey.length = " << itemKey.length()
			<< ", m_hashLength = "<< m_hashLength<< std::endl;
#endif		
    return (value * itemKey.length() ) % m_hashLength;
}

// Adds an item to the Hash Table.
void HashTable::insertItem( Item * newItem )
{
    int index = hash( newItem -> key );
    m_array[ index ].listInsertItem( newItem );
#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << ", index = " << index
			<< ", newItem->key = " << newItem->key 
			<< " addr = " << newItem << std::endl;
#endif
}

// Deletes an Item by key from the Hash Table.
// Returns true if the operation is successful.
bool HashTable::removeItem( string itemKey )
{
    int index = hash( itemKey );
#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << ", index = " << index
			<< ", itemKey = " << itemKey << std::endl;
#endif
    return m_array[ index ].listRemoveItem( itemKey );
}

// Returns an item from the Hash Table by key.
// If the item isn't found, a null pointer is returned.
Item * HashTable::getItemByKey( string itemKey )
{
    int index = hash( itemKey );
    return m_array[ index ].listGetItem( itemKey );
}

// Display the contents of the Hash Table to console window.
void HashTable::printTable()
{
    cout << "\n\nHash Table:\n";
    for ( int i = 0; i < m_hashLength; i++ )
    {
        cout << "Bucket " << i << ": ";
        m_array[i].listPrintList();
    }
}

// Prints a histogram illustrating the Item distribution.
void HashTable::printHistogram()
{
    cout << "\n\nHash Table Contains ";
    cout << getNumberOfItems() << " Items total\n";
    for ( int i = 0; i < m_hashLength; i++ )
    {
        cout << "Bucket" << i << " :\t";
        for ( int j = 0; j < m_array[i].listGetLength(); j++ )
            cout << " X";
        cout << "\n";
    }
}

// Returns the number of locations in the Hash Table.
int HashTable::getLength()
{
    return m_hashLength;
}

// Returns the number of Items in the Hash Table.
int HashTable::getNumberOfItems()
{
    int itemCount = 0;
    for ( int i = 0; i < m_hashLength; i++ )
    {
        itemCount += m_array[i].listGetLength();
    }
    return itemCount;
}

// De-allocates all memory used for the Hash Table.
HashTable::~HashTable()
{
    delete [] m_array;
#ifdef ENABLE_DEBUG
		static int i = 0;
		std::cout << __func__ << ": " << ++i << std::endl;
#endif		
}

//*****************************************************************
// End of File
//*****************************************************************
