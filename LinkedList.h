
#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"
#include<iostream>

using namespace std;

template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list

	int count;	//number of items in the list
public:


	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}

	bool isEmpty()
	{
		if (Head == nullptr) return true;
		else
		{
			return false;
		}
	}

	void DeleteNodeAtIndex(int index) {
		Node<T>* Prev = Head;
		Node<T>* temp = Prev;
		//Node<T>* nodetodelete = NULL;
		//Node<T>* afterdelete = NULL;

		if (Head == nullptr)
		{
			return;
		}
		for (int i=0; i<INT_MAX; i++)
		{
			if (Head != NULL)
			{
				if (i == index)
				{
					if (index == 0)
					{
						DeleteFirst();
						return;
					}
					else
					{
						temp = Prev->getNext();
						Prev->setNext(Prev->getNext()->getNext());
						delete temp;
						Prev = Prev->getNext();
						return;

					}
				}
			}
		}
		/*
		for (size_t i = 0; i < 999999999999; i++)
		{
			if(i==index)
			{
				delete temp;
			}

		}
		if (Prev->getNext() != NULL)
		{
			nodetodelete = Prev->getNext();
		}
		if (Prev->getNext()!=NULL) {
			afterdelete = Prev->getNext()->getNext();
			Prev->setNext(afterdelete);

		}
		*/
	}

	/*
	 Function: PrintList.
	 prints the values of all nodes in a linked list.
	*/

	void PrintList()	const
	{
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}

	/*
	Function: InsertBeg.
	Creates a new node and adds it to the beginning of a linked list.

	Parameters:
	Data : The value to be stored in the new node.
	*/

	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;
		R = nullptr;
		delete R;
		count++;
	}

	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/

	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}

	// Implement the following member functions

	//Access element 
	T Getelement(const int& kth)
	{
		int i = 1;
		Node<T>* current = Head;
		while (current != nullptr)
		{
			if (i == kth + 1)
			{
				//cout << current->getItem();
				return current->getItem();
			}
			current = current->getNext();
			i++;
		}
		//cout << "Beyond List Length";
	}

	//[1]InsertEnd 
	//inserts a new node at end of the list
	void InsertEnd(const T& data)
	{
		Node<T>* newNode = new Node<T>(data);
		if (Head == nullptr)
		{
			Head = newNode;
			return;
		}
		Node<T>* current = Head;
		while (current->getNext() != nullptr)
		{
			current = current->getNext();
		}
		current->setNext(newNode);
		delete newNode;
		newNode = nullptr;
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key)
	{
		Node<T>* current = Head;
		while (current != nullptr)
		{
			if (current->getItem() == Key)
			{
				return true;
			}
			current = current->getNext();
		}
		return false;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value)
	{
		Node<T>* current = Head;
		int count = 0;
		while (current != nullptr)
		{
			if (current->getItem() == value)
			{
				count++;
			}
			current = current->getNext();
		}
		return count;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst()
	{
		if (Head == nullptr)
		{
			return;
		}
		Node<T>* temp = Head;
		Head = Head->getNext();
		delete temp;
	}

	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast(T& temp)
	{
		temp;
		if (Head == nullptr)
		{
			temp = NULL;
			return;
		}
		if (Head->getNext() == nullptr)
		{
			temp = Head->getItem();
			delete Head;
			Head = nullptr;
			return;
		}
		Node<T>* current = Head;
		while (current->getNext()->getNext() != nullptr)
		{
			current = current->getNext();
		}
		temp = current->getNext()->getItem();
		delete current->getNext();
		current->setNext(nullptr);
		return;
	}

	//[6] DeleteNode
	//Deletes the first node with the given value (if found) and returns true, otherwise return false
	//Note: List is not sorted
	bool DeleteNode(const T& value) 
	{
		if (Head == nullptr) 
		{
			return false;
		}
		if (Head->getItem() == value) 
		{
			DeleteFirst();
			return true;
		}
		Node<T>* current = Head;
		while (current->getNext() != nullptr) 
		{
			if (current->getNext()->getItem() == value) 
			{
				Node<T>* temp = current->getNext();
				current->setNext(current->getNext()->getNext());
				delete temp;
				return true;
			}
			current = current->getNext();
		}
		return false;
	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true, otherwise return false
	//Note: List is not sorted
	bool DeleteNodes(const T& value) 
	{
		bool found = Find(value);
		while (Find(value)) 
		{
			DeleteNode(value);
		}
		return found;
	}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(LinkedList& L) 
	{
		Node<T>* current = Head;
		while (current->getNext() != nullptr) 
		{
			current = current->getNext();
		}
		current->setNext(L.Head);
		L.Head = nullptr;
	}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse() 
	{
		Node<T>* previous = nullptr;
		Node<T>* current = Head;
		Node<T>* next = nullptr;
		while (current != nullptr) 
		{
			next = current->getNext();
			current->setNext(previous);
			previous = current;
			current = next;
		}
		Head = previous;
	}
	void PrintKth(const int& kth) 
	{
		int i = 1;
		Node<T>* current = Head;
		while (current != nullptr) 
		{
			if (i == kth) 
			{
				cout << current->getItem();
				return;
			}
			current = current->getNext();
			i++;
		}
		cout << "Beyond List Length";
	}
	bool InsertSorted(const T& data) 
	{
		if (Head == nullptr || Head->getItem() > data) 
		{
			Node<T>* NewNode = new Node<T>;
			NewNode->setItem(data);
			NewNode->setNext(Head);
			Head = NewNode;
			delete NewNode;
			NewNode = nullptr;
			return true;
		}
		else 
		{
			Node<T>* current = Head;
			Node<T>* previous = nullptr;
			while (current != nullptr) 
			{
				if (current->getItem() > data) 
				{
					Node<T>* NewNode = new Node<T>;
					NewNode->setItem(data);
					NewNode->setNext(current);
					previous->setNext(NewNode);
					return true;
				}
				previous = current;
				current = current->getNext();
			}
			if (previous->getItem() < data) 
			{
				Node<T>* NewNode = new Node<T>;
				NewNode->setItem(data);
				previous->setNext(NewNode);
				return true;
			}
			return false;
		}
	}
	Node<T>* RemoveMin() 
	{
		if (Head == nullptr) 
		{
			return nullptr;
		}
		if (Head->getNext() == nullptr) 
		{
			Node<T>* temp = Head;
			Head = nullptr;
			return temp;
		}
		Node<T>* current = Head;
		Node<T>* min = Head;
		Node<T>* beforeMin = nullptr;
		Node<T>* previous = nullptr;
		while (current != nullptr) 
		{
			if (current->getItem() < min->getItem()) 
			{
				min = current;
				beforeMin = previous;
			}
			previous = current;
			current = current->getNext();
		}
		beforeMin->setNext(min->getNext());
		return min;
	}
	void SignSplit(LinkedList<T>& Lneg, LinkedList<T>& Lpos) 
	{
		Node<T>* current = Head;
		while (current != nullptr) 
		{
			T temp = current->getItem();
			if (temp > 0) 
			{
				Lpos.InsertSorted(temp);
			}
			else if (temp < 0) 
			{
				Lneg.InsertSorted(temp);
			}
			current = current->getNext();
			if (temp != 0) 
			{
				this->DeleteNode(temp);
			}
		}
	}
};

#endif