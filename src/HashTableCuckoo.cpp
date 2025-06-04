#include "../include/HashTableCuckoo.h"
#include <stdexcept>

// Konstruktor: alokuje dwie tablice i ustawia pola na puste
HashTableCuckoo::HashTableCuckoo(size_t size)
    : capacity(size), num_unoccupied(0) {
    table1 = new Entry[capacity];
    table2 = new Entry[capacity];
    for (size_t i = 0; i < capacity; ++i) {
        table1[i].occupied = false;
        table2[i].occupied = false;
    }
}

// Destruktor: zwalnia pamięć
HashTableCuckoo::~HashTableCuckoo() {
    delete[] table1;
    delete[] table2;
}

// Pierwsza funkcja haszująca
size_t HashTableCuckoo::hash1(int key) const {
    return (static_cast<size_t>(key) * 2654435761u) % capacity;
}

// Druga funkcja haszująca
size_t HashTableCuckoo::hash2(int key) const {
    return (static_cast<size_t>(key) * 40503u + 97u) % capacity;
}

// Wstawianie elementu z mechanizmem "cuckoo"
void HashTableCuckoo::insert(int key, int value) {
    int k = key, v = value;
    for (int kick = 0; kick < MAX_KICKS; ++kick) {
        size_t idx1 = hash1(k);
        if (!table1[idx1].occupied) {
            table1[idx1].key = k;
            table1[idx1].value = v;
            table1[idx1].occupied = true;
            return;
        }
        if (table1[idx1].key == k) {
            table1[idx1].value = v;
            return;
        }
        std::swap(k, table1[idx1].key);
        std::swap(v, table1[idx1].value);
        size_t idx2 = hash2(k);
        if (!table2[idx2].occupied) {
            table2[idx2].key = k;
            table2[idx2].value = v;
            table2[idx2].occupied = true;
            return;
        }
        if (table2[idx2].key == k) {
            table2[idx2].value = v;
            return;
        }
        std::swap(k, table2[idx2].key);
        std::swap(v, table2[idx2].value);
    }
    rehash(true); // Jeśli za dużo cykli, powiększ tablice
    insert(k, v);
}

// Usuwanie elementu z obu tablic
void HashTableCuckoo::remove(int key) {
    size_t idx1 = hash1(key);
    if (table1[idx1].occupied && table1[idx1].key == key) {
        table1[idx1].occupied = false;
        ++num_unoccupied;
        if (num_unoccupied > capacity / 4) {
            rehash(false); // Oczyszczanie tablicy
            num_unoccupied = 0;
        }
        return;
    }
    size_t idx2 = hash2(key);
    if (table2[idx2].occupied && table2[idx2].key == key) {
        table2[idx2].occupied = false;
        ++num_unoccupied;
        if (num_unoccupied > capacity / 4) {
            rehash(false);
            num_unoccupied = 0;
        }
        return;
    }
}

// Rehashing: powiększanie lub oczyszczanie tablic
void HashTableCuckoo::rehash(bool grow) {
    size_t new_capacity = grow ? (capacity * 2 + 1) : capacity;
    Entry* new_table1 = new Entry[new_capacity];
    Entry* new_table2 = new Entry[new_capacity];
    for (size_t i = 0; i < new_capacity; ++i) {
        new_table1[i].occupied = false;
        new_table2[i].occupied = false;
    }
    for (size_t i = 0; i < capacity; ++i) {
        if (table1[i].occupied) {
            int k = table1[i].key, v = table1[i].value;
            size_t idx = (static_cast<size_t>(k) * 2654435761u) % new_capacity;
            if (!new_table1[idx].occupied) {
                new_table1[idx].key = k;
                new_table1[idx].value = v;
                new_table1[idx].occupied = true;
            } else {
                // Jeśli miejsce zajęte, wrzuć do drugiej tablicy
                size_t idx2 = (static_cast<size_t>(k) * 40503u + 97u) % new_capacity;
                new_table2[idx2].key = k;
                new_table2[idx2].value = v;
                new_table2[idx2].occupied = true;
            }
        }
        if (table2[i].occupied) {
            int k = table2[i].key, v = table2[i].value;
            size_t idx = (static_cast<size_t>(k) * 2654435761u) % new_capacity;
            if (!new_table1[idx].occupied) {
                new_table1[idx].key = k;
                new_table1[idx].value = v;
                new_table1[idx].occupied = true;
            } else {
                size_t idx2 = (static_cast<size_t>(k) * 40503u + 97u) % new_capacity;
                new_table2[idx2].key = k;
                new_table2[idx2].value = v;
                new_table2[idx2].occupied = true;
            }
        }
    }
    delete[] table1;
    delete[] table2;
    table1 = new_table1;
    table2 = new_table2;
    capacity = new_capacity;
}

// Szukanie elementu w tablicy
bool HashTableCuckoo::find(int key, int& value) const {
    size_t idx1 = hash1(key);
    if (table1[idx1].occupied && table1[idx1].key == key) {
        value = table1[idx1].value;
        return true;
    }
    size_t idx2 = hash2(key);
    if (table2[idx2].occupied && table2[idx2].key == key) {
        value = table2[idx2].value;
        return true;
    }
    return false;
}

