#include "../include/HashTableAVL.h"
#include "../include/AVLTree.h"

// Konstruktor: alokuje tablicę wskaźników do drzew AVL
HashTableAVL::HashTableAVL(size_t size) : capacity(size) {
    table = new AVLTree*[capacity];
    for (size_t i = 0; i < capacity; ++i) table[i] = new AVLTree();
}

// Destruktor: zwalnia pamięć po wszystkich drzewach
HashTableAVL::~HashTableAVL() {
    for (size_t i = 0; i < capacity; ++i) delete table[i];
    delete[] table;
}

// Wstawianie elementu do odpowiedniego drzewa AVL
void HashTableAVL::insert(int key, int value) {
    size_t idx = hash(key);
    table[idx]->insert(key, value);
}

// Usuwanie elementu z odpowiedniego drzewa AVL
void HashTableAVL::remove(int key) {
    size_t idx = hash(key);
    table[idx]->remove(key);
}

// Funkcja haszująca (rozprasza klucze po kubełkach)
size_t HashTableAVL::hash(int key) const {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return static_cast<size_t>(key) % capacity;
}
// wyszukiwanie klucza
bool HashTableAVL::find(int key, int& value) const {
    size_t idx = hash(key);
    if (table[idx]) {
        return table[idx]->find(key, value);
    }
    return false;
}

