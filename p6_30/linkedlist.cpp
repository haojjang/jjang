// Author: Justin Fung

#include <iostream> // for NULL
#include "linkedlist.h"
#include "directory.h"

template <class T>
ListNode<T>::ListNode(T *d, ListNode *n): data(d), next(n)
{
} // ListNode()


template <class T>
ListNode<T>::~ListNode()
{
  delete data;
} // ~ListNode()


template <class T>
LinkedList<T>::LinkedList() : head(NULL)
{
} // LinkedList()


template <class T>
LinkedList<T>::~LinkedList()
{
  ListNode<T> *ptr;

  for(ptr = head; ptr; ptr = head)
  {
    head = ptr->next;
    delete ptr;
  } // for each ListNode
} // ~LinkedList()


template <class T>
const T* LinkedList<T>::operator[] (int index)const
{
  int i;
  ListNode<T> *ptr;

  for(i = 0, ptr = head; ptr && i < index; i++, ptr = ptr->next);

  return ptr->data;
} // operator[] const


template <class T>
T* LinkedList<T>::operator[] (int index)
{
  int i;
  ListNode<T> *ptr;

  for(i = 0, ptr = head; ptr && i < index; i++, ptr = ptr->next);

  return ptr->data;
} // operator[]


template <class T>
LinkedList<T>& LinkedList<T>::operator+= (T *data)
{
  ListNode<T> *ptr, *prev = NULL;

  for(ptr = head; ptr && *(ptr->data) < *data; ptr = ptr->next)
    prev = ptr;

  if(!prev)
    head = new ListNode<T>(data, head);
  else // prev exists
    prev->next = new ListNode<T>(data, ptr);

  return *this;
} // operator+=


/*
LinkedList& LinkedList::operator= (const LinkedList &linkedList)
{
  if(this == &linkedList)
    return *this;

  ListNode *ptr, *ptr2;

  for(ptr = head; ptr; ptr = head)
  {
    head = ptr->next;
    delete ptr;
  } // for each ListNode

  if(linkedList.head)
  {
    head = new ListNode(new Directory(*(linkedList.head->directory)), NULL);
    ptr = head;
    for(ptr2 = linkedList.head->next; ptr2; ptr2 = ptr2->next)
    {
      ptr->next = new ListNode(new Directory(*(ptr2->directory)), NULL);
      ptr = ptr->next;
    } // for each node in linkedList
  } // if entries in linkedList
  else
    head = NULL;

  return *this;

} // operator=
 */