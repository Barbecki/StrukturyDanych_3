#pragma once
#include <cstddef>

// Tablica mieszająca z Cuckoo Hashing
class HashTableCuckoo {
public:
    HashTableCuckoo(std::size_t size = 101);
    ~HashTableCuckoo();
    void insert(int key, int value);
    void remove(int key);
private:
    struct Entry {
        int key, value;
        bool occupied;
        Entry() : key(0), value(0), occupied(false) {}
    };
    std::size_t size_;
    Entry* table1_;
    Entry* table2_;
    std::size_t hash1(int key) const;
    std::size_t hash2(int key) const;
    void rehash();
    static const int MAX_LOOP = 32;
};
