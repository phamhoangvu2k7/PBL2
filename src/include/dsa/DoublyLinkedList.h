#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include <stdexcept> //Exception Handling
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
    void push_front(T val);
    void pop_back();
    void pop_front();
    bool remove(T val);
    bool insertAt(int index, T val);
    bool removeAt(int index);

    Node<T>* find(T val) const;
    bool contains(T val) const;
    T get(int index) const;

    int getSize() const;
    bool isEmpty() const;
    Node<T>* getHead() const;
    Node<T>* getTail() const;
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
void DoublyLinkedList<T>::push_front(T val) {
    Node<T>* newNode = new Node<T>(val);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
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
void DoublyLinkedList<T>::pop_front() {
    if (head == nullptr) return;
    Node<T>* temp = head;
    head = head->next;
    if (head != nullptr) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
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
bool DoublyLinkedList<T>::insertAt(int index, T val) {
    if (index < 0 || index > size) return false;
    if (index == 0) {
        push_front(val);
        return true;
    }
    if (index == size) {
        push_back(val);
        return true;
    }

    Node<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    Node<T>* newNode = new Node<T>(val);
    newNode->prev = current->prev;
    newNode->next = current;
    if (current->prev != nullptr) {
        current->prev->next = newNode;
    }
    current->prev = newNode;
    size++;
    return true;
}

template <typename T>
bool DoublyLinkedList<T>::removeAt(int index) {
    if (index < 0 || index >= size) return false;
    if (index == 0) {
        pop_front();
        return true;
    }
    if (index == size - 1) {
        pop_back();
        return true;
    }

    Node<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    size--;
    return true;
}

template <typename T>
Node<T>* DoublyLinkedList<T>::find(T val) const {
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == val) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

template <typename T>
bool DoublyLinkedList<T>::contains(T val) const {
    return find(val) != nullptr;
}

template <typename T>
T DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= size) {
        throw out_of_range("Index out of bounds");
    }
    Node<T>* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
int DoublyLinkedList<T>::getSize() const {
    return size;
}

template <typename T>
bool DoublyLinkedList<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
Node<T>* DoublyLinkedList<T>::getHead() const {
    return head;
}

template <typename T>
Node<T>* DoublyLinkedList<T>::getTail() const {
    return tail;
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
