/*
 * LinkedList.cc - A templated doubly linked list implementation.
 *
 * Date: 22 July 2001
 *
 * Copyright (C) 2000 Frank Hale
 * frankhale@phreaker.net
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

#include <stdio.h>
#include <stdlib.h> // my mandrake box needs this.
#include "linkedlist.hh"

_LinkedList::_LinkedList()
{
	// Get our linked list started. 
	// Do our default initialization here.

	head = NULL;
	tail = NULL;
	iter = NULL;

	elements=0;
}

_LinkedList::~_LinkedList()
{
	// remove each element 
	removeAll();
}

void _LinkedList::removeAll()
{
	Element *temp;		  //holds the element to be deleted
	while(head != NULL)	  //while anything is in the LL
	{
		temp = head;      //grab the top item
		head=head->next;  //move the next up
		delete temp;      //and delete the old top.
	}
	
	//now we know everything is gone, so reset to empty state
	tail=iter=0;
	elements=0;
}

Element* _LinkedList::getPrevElement()
{
	if(iter != NULL)
	{
		iter = iter->prev;

		return iter;
	} 

	return NULL;
}

Element* _LinkedList::getNextElement()
{
	if(iter != NULL)
	{
		iter = iter->next;

		return iter;
	} 

	return NULL;
}

// Inserts an element onto the end of the list.
void _LinkedList::insert(void *data)
{
	// We want to add an element by its data. We
	// don't want the user of the list to be concerned
	// about creating a new element his/her self.

	// Allocate memory for a new Element.
	Element *temp = new Element();

	// Set the temp element data to the data passed 
	// to this function.
	temp->data = data;

	// If head is NULL we have an empty list.
	if(head==NULL) 
	{
		// If the list is empty then insert it at
		// the beginning of the list.
		head = tail = temp;
		temp->prev = NULL;

	} else {
		
		// List has elements in it already,
		// Insert onto the end of the list.
		tail->next = temp;
		temp->prev = tail;
	}

	// Remember that this element is 
	// at the end of the list.
	tail = temp;
	temp->next = NULL;

	// Add one to the number of elements.
	elements++;
}

Element* _LinkedList::find(void *data)
{
	Element *temp = NULL;

	// This for loop iterates through the elements
	// so we can check for the data. If we find data
	// then we found our element.
	for (temp = head; temp != NULL; temp = temp->next)
	{
		// Return the element if we find its data 
		if (temp->data == data) return temp;
	}

	// The data doesn't belong to any element in
	// our list.
	return NULL;
}

void _LinkedList::remove(void *data)
{
	// We can't delete an element if 
	// that element doesn't exist in 
	// the first place.
	Element *temp = find(data);

	// If temp element is NULL that means
	// we didn't find the element we were
	// looking for.
	if(temp == NULL) 
	{
		printf("_LinkedList::remove : element not found\n");
		return;
	
	} else {

		// On the off chance there is duplicates get rid of them.
		while(temp) 
		{	
			if(temp==head) head = temp->next;    //if head just drop off
			else temp->prev->next = temp->next;  //else drop out.

			if(temp==tail) tail = temp->prev;    //if tail just drop off
			else temp->next->prev = temp->prev;  //else drop out
		
			delete temp;
		
			// Subtract one from total elements.
			elements--;

			// Look again to see if there is a duplicate
			temp = find(data);
		}
	}
}

/* _LinkedListIterator Below */

_LinkedListIterator::_LinkedListIterator()
{
}

_LinkedListIterator::_LinkedListIterator(_LinkedList *l, int direction)
{ 
	setList(l, direction);
}

void _LinkedListIterator::setList(_LinkedList *l, int direction)
{
	if(l != NULL)
	{
		list = l; 
		element = NULL;
		
		reset(direction);
	} else {
			printf("_LinkedListIterator: List is NULL\n");
			exit(-1);
	}
}	

void _LinkedListIterator::reset(int direction)
{
		if(direction == FORWARD) 
		{
			list->setForward();
			element = list->head;
		} 
		else if(direction == BACKWARD) 
		{
			list->setBackward();
			element = list->tail;
		} else {
			printf("_LinkedListIterator: Illegal direction for list traversal using FORWARD\n");

			list->setForward();
			element = list->head;
		}
}

void _LinkedListIterator::operator++(int)
{
	if(element) element = list->getNextElement();
}

void _LinkedListIterator::operator--(int)
{
	if(element) element = list->getPrevElement();
}

void* _LinkedListIterator::current() 
{
//    if(element) fprintf(stderr, "LL: Have element!\n");
//    else        fprintf(stderr, "LL: No element!\n");
  
  return ( (element && element->data) ? element->data : NULL);
}

/* --------------------------------------------------------------------- */
/* Example of usage of the LinkedList and LinkedListIterator classes below */
/*

class mydata
{
public:
	char* name;
};

int main()
{
	LinkedList<mydata> *test = new LinkedList<mydata>;
	  
	mydata *one = new mydata();
	mydata *two = new mydata();
	mydata *three = new mydata();
	mydata *four = new mydata();
	
	one->name = "1. Frank";
	two->name = "2. John";
	three->name = "3. Joe";
	four->name = "4. Mike";
	
	test->insert(one);
	test->insert(two);
	test->insert(three);
	test->insert(four);
	
	printf("Printing List Forward\n\n");

	LinkedListIterator<mydata> it(test);
	for(; it.current(); it++)
	{
		printf("%s\n", it.current()->name);
	}

	printf("\nPrinting List Backwards\n\n");

	it.reset(BACKWARD);
	
	for(; it.current(); it--)
	{
		printf("%s\n", it.current()->name);
	}

	printf("\nTotal Elements: %d\n\n", test->getElementCount());

	delete one;
	delete two;
	delete three;
	delete four;

	delete test;

return 0;
}
*/
