#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <vector>
#include "../dsa/DoublyLinkedList.h"
#include "../dsa/HashTable.h"
#include "../helper/RecordFile.h"
using namespace std;

template <typename T>
class Repository {
private:
    DoublyLinkedList<T*> items;
    HashTable<string, T*> index;
    string filePath; 

    // luu vao ctdl
    bool put(T* item) {
        string id = idOf(*item);
        //validate
        if (id.empty() || index.containsKey(id) || !onAdd(item)) return false;
        items.push_back(item);
        index.insert(id, item);
        return true;
    }

protected:
    virtual string idOf(const T& item) const = 0;
    virtual vector<string> toRecord(const T& item) const = 0;
    virtual T* fromRecord(const vector<string>& record) const = 0;  

    virtual bool onAdd(T*) { return true; }

public:
    explicit Repository(const string& file) : filePath(file) {}

    virtual ~Repository() {
        for (Node<T*>* cur = items.getHead(); cur != nullptr; cur = cur->next) delete cur->data;
    }

    //Save 
    bool add(T* item) {
        if (item == nullptr || !put(item)) return false;
        save();
        return true;
    }

    //update
    bool update(T* item) {
        return item != nullptr && save();
    }

    T* findById(const string& id) const {
        T* result = nullptr;
        index.find(id, result);
        return result;
    }

    //get data
    const DoublyLinkedList<T*>& getAll() const {
        return items;
    }

    int size() const { return items.getSize(); }

    vector<T*> toVector() const {
        vector<T*> out;
        for (Node<T*>* cur = items.getHead(); cur != nullptr; cur = cur->next) out.push_back(cur->data);
        return out;
    }

    //doc file luu vao ctdl
    bool load() {
        vector<vector<string>> records;
        if (filePath.empty() || !RecordFile::read(filePath, records)) return false;

        for (const vector<string>& r : records) {
            T* item = fromRecord(r);
            if (item != nullptr && !put(item)) delete item;
        }
        return true;
    }

    // luu xuong file
    bool save() const {
        if (filePath.empty()) return false;
        vector<string> lines;
        for (Node<T*>* cur = items.getHead(); cur != nullptr; cur = cur->next)
            lines.push_back(RecordFile::join(toRecord(*cur->data)));
        return RecordFile::write(filePath, lines);
    }
};

#endif
