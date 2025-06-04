#pragma once
#include "../include/IDictionary.h"
#include <cstddef>
// pliki nagłówkowe dla tablicy haszującej z haszowaniem cuckoo
class HashTableCuckoo : public IDictionary {
public:
    HashTableCuckoo(size_t size = 101);
    ~HashTableCuckoo();
    void insert(int key, int value) override;
    void remove(int key) override;
    HashTableCuckoo(const HashTableCuckoo&) = delete;
    HashTableCuckoo& operator=(const HashTableCuckoo&) = delete;
private:
    struct Entry {
        int key;
        int value;
        bool occupied;
        Entry() : key(0), value(0), occupied(false) {}
    };
    Entry* table1;
    Entry* table2;
    size_t capacity;
    size_t num_unoccupied;
    size_t hash1(int key) const;
    size_t hash2(int key) const;
    void rehash(bool grow);
    static const int MAX_KICKS = 32;
};
