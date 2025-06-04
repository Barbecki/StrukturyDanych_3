#pragma once
#include "../include/IDictionary.h"
#include "../include/AVLTree.h"
#include <cstddef>
// plik nagłówkowy dla tablicy haszującej z drzewami AVL
class HashTableAVL : public IDictionary {
public:
    HashTableAVL(size_t size = 101);
    ~HashTableAVL();
    void insert(int key, int value) override;
    void remove(int key) override;
    bool find(int key, int& value) const override;
    HashTableAVL(const HashTableAVL&) = delete;
    HashTableAVL& operator=(const HashTableAVL&) = delete;
private:
    AVLTree** table;
    size_t capacity;
    size_t hash(int key) const;
};
