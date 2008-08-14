/*
 * linklist.hh - This is a templated doubly linked list implementation with
 *				  a List class and a ListIterator class.
 *
 * Date: 22 July 2001
 *
 * Copyright (C) 2000 Frank Hale
 * frankhale@yahoo.com
 * http://sapphire.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

// Forward declaration. LinkedListIterator needs to be a friend of LinkedList.
class _LinkedListIterator;
class _LinkedList;

// Directions you can traverse the list
const int FORWARD  = 0;	
const int BACKWARD = 1;


/* will this break things? */
#ifndef NULL
#define NULL 0
#endif

// Element is an node in the list.
class Element
{
public: 

	// This is pretty nasty but I am using friend classed
	// to circumvent having to write get/set functions, but
	// I wanna make the next,prev, and data pointers private.
	// But I still need LinkedList and LinkedListIterator
	// to access those pointers.
	friend struct _LinkedList;
	friend struct _LinkedListIterator;

private: // Variables 
	
	Element *next; // Pointer to next element 
	Element *prev; // Pointer to previous element 
	void	*data; // The data that is stored in this element 

public: // Member functions 
	Element() 
	{ 
		next=NULL; 
		prev=NULL; 
		data=NULL; 
	}
};

// LinkedList is a list of elements.
class _LinkedList 
{
private: // Variables 
	Element *head, *tail, *iter;
	int elements;

	friend struct _LinkedListIterator;

private: // Member functions 
	Element* getNextElement(); // Returns the next element     
	Element* getPrevElement(); // Returns the previous element 

	// The next two functions set the direction of traversal 
	// This is for iterating purposes and functions with 
	// LinkedListIterator operator++ and operator--.
	void setForward()  { iter = head; }  
	void setBackward() { iter = tail; }

public: // Member functions
	_LinkedList();
	~_LinkedList();
	
	void* getHead() { return head->data; }
	void* getTail() { return tail->data; }
	
	void insert(void *data); // Inserts an element by data (NOT NODE) 
	void remove(void *data); // Removes an element based on its data  
	
	void removeAll();        // Remove all elements in this list. Note
				 // that the destructor will do clean up for
				 // you so you don't have to explicity call
			         // this function.
							 

	Element* find(void *data); // Finds an element by its data 

	// Returns the number of elements in this list 
	int getElementCount() const { return elements; }
	int count() const { return elements; }
};

// LinkedListIterator iterates a list so you can do something with each element.
class _LinkedListIterator 
{
private: // Variables 
	_LinkedList *list;
	Element *element;

public: // Member functions 
	
	_LinkedListIterator();
	
	// Constructor is passed a list and the desired direction of 
	// tranversal. The direction FORWARD is assumed.
	_LinkedListIterator(_LinkedList *l, int direction = FORWARD);
	
	// The next two functions iterate the list either forward
	// or backward depending on the direction you specified.
	void operator++(int);
	void operator--(int);

	// Reset interation to the beginning depending on the direction
	// you desire. 
	void reset(int direction = FORWARD);

	void setList(_LinkedList *l, int direction = FORWARD);

	void* current(); 
};

template <class T>
class LinkedList : public _LinkedList
{
public:
	LinkedList() : _LinkedList() { }
	
	T* first() { return (T*) _LinkedList::getHead(); }
	T* last()  { return (T*) _LinkedList::getTail(); }
	
	void insert(T *d) { _LinkedList::insert( (T*) d); }
	void remove(T *d) { _LinkedList::remove( (T*) d); }
};

template <class T>
class LinkedListIterator : public _LinkedListIterator
{
public:
	LinkedListIterator() : _LinkedListIterator() {}
	
	LinkedListIterator(_LinkedList *l, int direction = FORWARD) 
		: _LinkedListIterator(l, direction){}

	T* current() { return (T*) _LinkedListIterator::current(); }
};

#endif
