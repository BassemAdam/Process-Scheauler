#pragma once

#ifndef _NODE
#define _NODE
template < typename T>
class NodeQ
{
private:
	T item; // A data item
	NodeQ<T>* next; // Pointer to next node
public:
	NodeQ();
	NodeQ(const T& r_Item);
	NodeQ(const T& r_Item, NodeQ<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setNext(NodeQ<T>* nextNodePtr);
	T getItem() const;
	NodeQ<T>* getNext() const;
}; // end Node
#endif

template < typename T>
NodeQ<T>::NodeQ()
{
	next = nullptr;
}

template < typename T>
NodeQ<T>::NodeQ(const T& r_Item)
{
	item = r_Item;
	next = nullptr;
}

template < typename T>
NodeQ<T>::NodeQ(const T& r_Item, NodeQ<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void NodeQ<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void NodeQ<T>::setNext(NodeQ<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T NodeQ<T>::getItem() const
{
	return item;
}

template < typename T>
NodeQ<T>* NodeQ<T>::getNext() const
{
	return next;
}