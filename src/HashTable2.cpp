#include <cstddef>
#include "../include/HashTable2.hpp"

HashTableCuckoo::HashTableCuckoo(std::size_t size) : size_(size) {
    table1_ = new Entry[size_];
    table2_ = new Entry[size_];
}
HashTableCuckoo::~HashTableCuckoo() {
    delete[] table1_;
    delete[] table2_;
}
std::size_t HashTableCuckoo::hash1(int key) const { return static_cast<std::size_t>(key) % size_; }
std::size_t HashTableCuckoo::hash2(int key) const { return (static_cast<std::size_t>(key) / size_ + 17) % size_; }
void HashTableCuckoo::rehash() {
    Entry* old1 = table1_;
    Entry* old2 = table2_;
    std::size_t oldSize = size_;
    size_ *= 2;
    table1_ = new Entry[size_];
    table2_ = new Entry[size_];
    for (std::size_t i = 0; i < oldSize; ++i) {
        if (old1[i].occupied) insert(old1[i].key, old1[i].value);
        if (old2[i].occupied) insert(old2[i].key, old2[i].value);
    }
    delete[] old1;
    delete[] old2;
}
void HashTableCuckoo::insert(int key, int value) {
    int k = key, v = value;
    for (int loop = 0; loop < MAX_LOOP; ++loop) {
        std::size_t idx1 = hash1(k);
        if (!table1_[idx1].occupied) {
            table1_[idx1].key = k;
            table1_[idx1].value = v;
            table1_[idx1].occupied = true;
            return;
        }
        // swap k/v z tym co było w table1
        int tmpk = table1_[idx1].key, tmpv = table1_[idx1].value;
        table1_[idx1].key = k; table1_[idx1].value = v;
        k = tmpk; v = tmpv;
        std::size_t idx2 = hash2(k);
        if (!table2_[idx2].occupied) {
            table2_[idx2].key = k;
            table2_[idx2].value = v;
            table2_[idx2].occupied = true;
            return;
        }
        // swap k/v z tym co było w table2
        tmpk = table2_[idx2].key; tmpv = table2_[idx2].value;
        table2_[idx2].key = k; table2_[idx2].value = v;
        k = tmpk; v = tmpv;
    }
    rehash();
    insert(k, v);
}
void HashTableCuckoo::remove(int key) {
    std::size_t idx1 = hash1(key);
    if (table1_[idx1].occupied && table1_[idx1].key == key) {
        table1_[idx1].occupied = false;
        return;
    }
    std::size_t idx2 = hash2(key);
    if (table2_[idx2].occupied && table2_[idx2].key == key) {
        table2_[idx2].occupied = false;
        return;
    }
}
