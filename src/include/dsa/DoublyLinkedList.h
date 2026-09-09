#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(T val) {
        data = val;
        next = nullptr;
        prev = nullptr;
    }
};

template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~DoublyLinkedList() {
        clear();
    }

    void push_back(T val) {
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

    void pop_back() {
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

    bool remove(T val) {
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

    int getSize() const {
        return size;
    }

    Node<T>* getHead() const {
        return head;
    }

    void clear() {
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
};

#endif
