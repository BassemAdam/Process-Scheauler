#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class PriorityQueue {
public:
    PriorityQueue() : head(nullptr) {}
    ~PriorityQueue();
    bool peek(T& frntEntry)  const;
    void insert(const T& value, int priority);
    T remove();
    bool isEmpty() const;
    void Print();
private:
    struct Node {
        T value;
        int priority;
        Node* next;

        Node(const T& value, int priority, Node* next = nullptr)
            : value(value), priority(priority), next(next) {}

        T getItem() const
        {
            return value;
        } // end getItem
    };

    Node* head;
};

/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.
Input: None.
Output: The front of the queue.
*/

template <typename T>
bool PriorityQueue<T>::peek(T& frntEntry) const
{
    if (isEmpty())
        return false;

    frntEntry = head->getItem();
    return true;
}

template<typename T>
void PriorityQueue<T>:: Print()
{
    Node* ptr = head;
    if(head==NULL)
    {
        return;
    }
    else
    {
        while (ptr!=NULL)
        {
            cout << *ptr->value;
            if (ptr->next != NULL)
            {
                cout << ", ";
            }
            
            ptr = ptr->next;
        }
    }

}

template <typename T>
PriorityQueue<T>::~PriorityQueue() {
    while (!isEmpty()) {
        remove();
    }
}

template <typename T>
void PriorityQueue<T>::insert(const T& value, int priority) {

    if (isEmpty() || priority < head->priority) {
        head = new Node(value, priority, head);
        return;
    }

    Node* current = head;
    while (current->next != nullptr && priority >= current->next->priority) {
        current = current->next;
    }

    current->next = new Node(value, priority, current->next);
}

template <typename T>
T PriorityQueue<T>::remove() {
    if (isEmpty()) {
        //throw std::runtime_error("Trying to remove from an empty priority queue.");
        return NULL;
    }

    Node* temp = head;
    T value = temp->value;
    head = head->next;
    delete temp;
    return value;
}

template <typename T>
bool PriorityQueue<T>::isEmpty() const {
    return head == nullptr;
}

