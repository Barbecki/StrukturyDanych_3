
#include "../include/HashTableOpenAddressing.h"
#include <stdexcept>
#include <cmath>

// Sprawdza czy liczba jest pierwsza
static bool isPrime(size_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    for (size_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Znajdź najbliższą liczbę pierwszą >= n
static size_t nextPrime(size_t n) {
    while (!isPrime(n)) ++n;
    return n;
}

// Konstruktor: alokuje tablicę i ustawia stany na puste
HashTableOpenAddressing::HashTableOpenAddressing(size_t size)
    : table(nullptr), capacity(0), num_elements(0), num_deleted(0) {
    capacity = nextPrime(size);
    table = new Entry[capacity];
    for (size_t i = 0; i < capacity; ++i) table[i].state = State::EMPTY;
}

// Destruktor: zwalnia pamięć
HashTableOpenAddressing::~HashTableOpenAddressing() {
    delete[] table;
}

// Podstawowa funkcja haszująca
size_t HashTableOpenAddressing::hash(int key) const {
    size_t h = static_cast<size_t>(key);
    h ^= (h >> 16);
    return h % capacity;
}

// Funkcja do podwójnego haszowania (gwarantuje względną pierwszość)
size_t HashTableOpenAddressing::hash2(int key) const {
    size_t h = static_cast<size_t>(key);
    h ^= (h >> 8);
    return 1 + (h % (capacity - 1));
}

// Wstawianie elementu do tablicy
void HashTableOpenAddressing::insert(int key, int value) {
    if ((double)(num_elements + 1) / capacity > 0.8) {
        rehash(true); // Powiększ tablicę jeśli za dużo elementów
    }
    size_t idx = hash(key);
    size_t step = hash2(key);
    for (size_t i = 0; i < capacity; ++i) {
        size_t probe = (idx + i * step) % capacity;
        if (table[probe].state == State::EMPTY || table[probe].state == State::DELETED) {
            table[probe].key = key;
            table[probe].value = value;
            table[probe].state = State::OCCUPIED;
            ++num_elements;
            return;
        }
        if (table[probe].state == State::OCCUPIED && table[probe].key == key) {
            table[probe].value = value;
            return;
        }
    }
    throw std::overflow_error("Hash table is full");
}

// Usuwanie elementu z tablicy
void HashTableOpenAddressing::remove(int key) {
    size_t idx = hash(key);
    size_t step = hash2(key);
    for (size_t i = 0; i < capacity; ++i) {
        size_t probe = (idx + i * step) % capacity;
        if (table[probe].state == State::OCCUPIED && table[probe].key == key) {
            table[probe].state = State::DELETED;
            --num_elements;
            ++num_deleted;
            if (num_deleted > capacity / 4) {
                rehash(false); // Oczyszczanie tablicy
                num_deleted = 0;
            }
            return;
        }
        if (table[probe].state == State::EMPTY) {
            return;
        }
    }
}

// Rehashing: powiększanie lub oczyszczanie tablicy
void HashTableOpenAddressing::rehash(bool grow) {
    size_t new_capacity = grow ? nextPrime(capacity * 2 + 1) : capacity;
    Entry* new_table = new Entry[new_capacity];
    for (size_t i = 0; i < new_capacity; ++i) new_table[i].state = State::EMPTY;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == State::OCCUPIED) {
            size_t idx = static_cast<size_t>(table[i].key);
            idx ^= (idx >> 16);
            idx = idx % new_capacity;
            size_t step = 1 + ((static_cast<size_t>(table[i].key) ^ (static_cast<size_t>(table[i].key) >> 8)) % (new_capacity - 1));
            // Sondowanie do znalezienia wolnego miejsca
            for (size_t j = 0; j < new_capacity; ++j) {
                size_t probe = (idx + j * step) % new_capacity;
                if (new_table[probe].state == State::EMPTY) {
                    new_table[probe] = table[i];
                    break;
                }
            }
        }
    }
    delete[] table;
    table = new_table;
    capacity = new_capacity;
}

size_t HashTableOpenAddressing::countOccupied() const {
    size_t count = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i].state == State::OCCUPIED) ++count;
    }
    return count;
}

