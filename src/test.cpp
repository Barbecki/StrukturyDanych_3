#include "../include/HashTableOpenAddressing.h"
#include "../include/HashTableCuckoo.h"
#include "../include/HashTableAVL.h"
#include <iostream>

void test_dict(IDictionary& dict, const char* name) {
    std::cout << "Test: " << name << std::endl;
    dict.insert(1, 100);
    dict.insert(2, 200);
    dict.insert(3, 300);
    dict.insert(3, 200);
    dict.insert(7, 200);
    dict.insert(15, 600);
    dict.insert(64, 700);
    dict.insert(13, 100);
    dict.insert(5, 400);
    dict.insert(66, 400);
    int val;
    // Sprawdź find po insercie
    std::cout << "find(1): " << (dict.find(1, val) && val == 100 ? "OK" : "FAIL") << std::endl;
    std::cout << "find(2): " << (dict.find(2, val) && val == 200 ? "OK" : "FAIL") << std::endl;
    std::cout << "find(3): " << (dict.find(3, val) && val == 200 ? "OK" : "FAIL") << std::endl;
    std::cout << "find(66): " << (dict.find(66, val) && val == 400 ? "OK" : "FAIL") << std::endl;
    std::cout << "find(4): " << (!dict.find(4, val) ? "OK" : "FAIL") << std::endl;
    // Usuń i sprawdź
    dict.remove(2);
    std::cout << "remove(2), find(2): " << (!dict.find(2, val) ? "OK" : "FAIL") << std::endl;
    dict.remove(1);
    std::cout << "remove(1), find(1): " << (!dict.find(1, val) ? "OK" : "FAIL") << std::endl;
    dict.remove(3);
    std::cout << "remove(3), find(3): " << (!dict.find(3, val) ? "OK" : "FAIL") << std::endl;
    std::cout << std::endl;
}

int main() {
    HashTableOpenAddressing hto(17);
    HashTableCuckoo htc(17);
    HashTableAVL htavl(17);
    test_dict(hto, "OpenAddressing");
    test_dict(htc, "Cuckoo");
    test_dict(htavl, "AVLBucket");
    return 0;
}
