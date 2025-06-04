#pragma once
#include "../include/IDictionary.h"
#include <cstddef>
// pliki nagłówkowe dla tablicy haszującej z otwartym adresowaniem
class HashTableOpenAddressing : public IDictionary {
public:
    HashTableOpenAddressing(size_t size = 101);
    void insert(int key, int value) override;
    void remove(int key) override;
    size_t countOccupied() const;
    HashTableOpenAddressing(const HashTableOpenAddressing&) = delete;
    HashTableOpenAddressing& operator=(const HashTableOpenAddressing&) = delete;
    ~HashTableOpenAddressing();
private:
    enum class State { EMPTY, OCCUPIED, DELETED };
    struct Entry {
        int key;
        int value;
        State state;
        Entry() : key(0), value(0), state(State::EMPTY) {}
    };
    Entry* table;
    size_t capacity;
    size_t num_elements;
    size_t num_deleted;
    size_t hash(int key) const;
    size_t hash2(int key) const;
    void rehash(bool grow);
};
