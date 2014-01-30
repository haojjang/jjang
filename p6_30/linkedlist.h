// Author: Justin Fung

#ifndef linkedlistH
  #define linkedlistH

template <class T>
class LinkedList;

template <class T>
class ListNode
{
  T *data;
  ListNode *next;
  ListNode(T *d, ListNode *n);
  ~ListNode();
  friend class LinkedList<T>;

}; // ListNode class


template <class T>
class LinkedList
{
  ListNode<T> *head;
public:
  LinkedList();
  ~LinkedList();
  const T* operator[] (int index)const;
  T* operator[] (int index);
  LinkedList<T>& operator+= (T *data);
  //LinkedList& operator= (const LinkedList &linkedList);
}; // LinkedList class

#include "linkedlist.cpp"

#endif