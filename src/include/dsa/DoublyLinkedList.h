#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(T val);
};

template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void push_back(T val);
    void pop_back();
    bool remove(T val);
    int getSize() const;
    Node<T>* getHead() const;
    void clear();
};

template <typename T>
Node<T>::Node(T val) {
    data = val;
    next = nullptr;
    prev = nullptr;
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template <typename T>
void DoublyLinkedList<T>::push_back(T val) {
    Node<T>* newNode = new Node<T>(val);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

template <typename T>
void DoublyLinkedList<T>::pop_back() {
    if (tail == nullptr) return;
    Node<T>* temp = tail;
    tail = tail->prev;
    if (tail != nullptr) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    delete temp;
    size--;
}

template <typename T>
bool DoublyLinkedList<T>::remove(T val) {
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == val) {
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }

            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
int DoublyLinkedList<T>::getSize() const {
    return size;
}

template <typename T>
Node<T>* DoublyLinkedList<T>::getHead() const {
    return head;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T>* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

#endif
