#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>
using namespace std;

template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;
    HashNode* prev;

    HashNode(K k, V v);
};

template <typename K, typename V>
class HashTable {
private:
    static const int TABLE_SIZE = 101;
    HashNode<K, V>* table[TABLE_SIZE];
    int size;

    int hashFunc(const string& key) const;
    int hashFunc(int key) const;

public:
    HashTable();
    ~HashTable();

    void insert(K key, V value);
    bool find(K key, V& outValue) const;
    bool remove(K key);
    int getSize() const;
    void clear();
};

template <typename K, typename V>
HashNode<K, V>::HashNode(K k, V v) {
    key = k;
    value = v;
    next = nullptr;
    prev = nullptr;
}

template <typename K, typename V>
HashTable<K, V>::HashTable() {
    size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    clear();
}

template <typename K, typename V>
int HashTable<K, V>::hashFunc(const string& key) const {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % TABLE_SIZE;
    }
    if (hash < 0) hash += TABLE_SIZE;
    return hash;
}

template <typename K, typename V>
int HashTable<K, V>::hashFunc(int key) const {
    int hash = key % TABLE_SIZE;
    if (hash < 0) hash += TABLE_SIZE;
    return hash;
}

template <typename K, typename V>
void HashTable<K, V>::insert(K key, V value) {
    int index = hashFunc(key);
    HashNode<K, V>* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
    newNode->next = table[index];
    if (table[index] != nullptr) {
        table[index]->prev = newNode;
    }
    table[index] = newNode;
    size++;
}

template <typename K, typename V>
bool HashTable<K, V>::find(K key, V& outValue) const {
    int index = hashFunc(key);
    HashNode<K, V>* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            outValue = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(K key) {
    int index = hashFunc(key);
    HashNode<K, V>* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                table[index] = current->next;
            }

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            }

            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename K, typename V>
int HashTable<K, V>::getSize() const {
    return size;
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode<K, V>* current = table[i];
        while (current != nullptr) {
            HashNode<K, V>* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    size = 0;
}

#endif
