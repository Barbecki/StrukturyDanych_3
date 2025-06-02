#pragma once
#include <cstddef>
#include "AVL.hpp"

template<typename Key, typename Value, typename BucketType>
class HashTable {
public:
    HashTable(std::size_t size = 101);
    ~HashTable();
    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    bool find(const Key& key, Value& out) const;
private:
    std::size_t size_;
    BucketType** buckets_;
    std::size_t hash(const Key& key) const;
};

// --- IMPLEMENTACJA SZABLONU TUTAJ ---
template<typename Key, typename Value, typename BucketType>
HashTable<Key, Value, BucketType>::HashTable(std::size_t size) : size_(size) {
    buckets_ = new BucketType*[size_];
    for (std::size_t i = 0; i < size_; ++i) buckets_[i] = nullptr;
}
template<typename Key, typename Value, typename BucketType>
HashTable<Key, Value, BucketType>::~HashTable() {
    for (std::size_t i = 0; i < size_; ++i) delete buckets_[i];
    delete[] buckets_;
}
template<typename Key, typename Value, typename BucketType>
void HashTable<Key, Value, BucketType>::insert(const Key& key, const Value& value) {
    std::size_t idx = hash(key) % size_;
    if (!buckets_[idx]) buckets_[idx] = new BucketType();
    buckets_[idx]->insert(key, value);
}
template<typename Key, typename Value, typename BucketType>
void HashTable<Key, Value, BucketType>::remove(const Key& key) {
    std::size_t idx = hash(key) % size_;
    if (buckets_[idx]) buckets_[idx]->remove(key);
}
template<typename Key, typename Value, typename BucketType>
bool HashTable<Key, Value, BucketType>::find(const Key& key, Value& out) const {
    std::size_t idx = hash(key) % size_;
    if (buckets_[idx]) return buckets_[idx]->find(key, out);
    return false;
}
template<typename Key, typename Value, typename BucketType>
std::size_t HashTable<Key, Value, BucketType>::hash(const Key& key) const {
    return static_cast<std::size_t>(key);
}
