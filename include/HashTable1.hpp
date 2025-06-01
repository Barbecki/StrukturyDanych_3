#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

class HashTable {
private:
    struct Element {
        int key;
        int value;
        Element(int k = -1, int v = 0);
    };

    Element* table;
    int capacity;
    int size;

    int hash(int key) const;

public:
    HashTable(int c = 101);
    ~HashTable();

    bool insert(int key, int value);
    bool remove(int key);
    void print() const;
};



#endif //HASHTABLE_HPP
