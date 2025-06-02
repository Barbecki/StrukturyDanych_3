#include "../include/HashTable1.hpp"
#include <iostream>

const int EMPTY = -1;
const int DELETED = -2;

//Konstruktor elementu (kubełka)
HashTable::Element::Element(int k, int v) : key(k), value(v) {}

//Konstruktor tabeli
HashTable::HashTable(int c) : capacity(c), size(0) {

    table = new Element[capacity];
}

//Destruktor
HashTable::~HashTable() {
    delete[] table;
}

//funkcja mieszajaca
int HashTable::hash(int key) const {
    return key % capacity;
}

//dodawanie elementu
bool HashTable::insert(int key, int value) {
    int index = hash(key);
    int startIndex = index;

    do {
        if (table[index].key == EMPTY || table[index].key == DELETED) {
            table[index] = Element(key, value);
            size++;
            return true;
        } else if (table[index].key == key) {
            table[index].value = value;
            return true;
        }
        index = (index + 1) % capacity;
    } while (index != startIndex);

    std::cout << "Tablica mieszajaca jest pelna\n";
    return false;
}

//usuwanie elementu
bool HashTable::remove(int key) {
    int index = hash(key);
    int startIndex = index;

    do {
        if (table[index].key == key) {
            table[index].key = DELETED;
            size--;
            return true;
        } else if (table[index].key == EMPTY) {
            return false;
        }
        index = (index + 1) % capacity;
    } while (index != startIndex);

    return false;
}
