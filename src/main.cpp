#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <exception>
#include <algorithm>
#include "../include/HashTableOpenAddressing.h"
#include "../include/HashTableCuckoo.h"
#include "../include/HashTableAVL.h"


int main() {
    std::ofstream csv("results.csv");
    csv << "size,structure,avg_insert_time_ns,avg_remove_time_ns\n";
    // Rozmiary 
    int sizes[8] = {10000, 45000, 80000, 115000, 150000, 185000, 220000, 255000};
    std::uniform_int_distribution<int> dist(1, 10000000);
   // Współczynnik zapełnienia dla testów
    double fillV = 0.8;



    const int N = 100; 
    for (int s = 0; s < 8; ++s) {
        int size = sizes[s];
        long long total_insert_OA = 0, total_remove_OA = 0;
        long long total_insert_Cuckoo = 0, total_remove_Cuckoo = 0;
        long long total_insert_AVL = 0, total_remove_AVL = 0;
        int errors_OA = 0;
        std::cout << "Test size: " << size << std::endl;
        for (int t = 0; t < N; ++t) {
            std::mt19937 rng(size+t);
            int fill_count = int(size*fillV);
            std::vector<int> keys(fill_count); // klucze
            std::vector<int> values(fill_count); // wartości

            // wypełnienie tablicy losowymi danymi ( unikatowe klucze i losowe wartości)
            std::iota(keys.begin(), keys.end(), t*size);
            std::shuffle(keys.begin(), keys.end(), rng);
            for (int i = 0; i < 1; ++i) {
                values[i] = dist(rng);
            }
            int key = size*100 + t;
            int value = dist(rng);

            // OpenAddressing
            try {
                HashTableOpenAddressing table(size);
                for (int i = 0; i < fill_count; ++i) {
                    table.insert(keys[i], values[i]);
                }
                table.insert(key, value);
                table.remove(key);
                auto start = std::chrono::high_resolution_clock::now();
                table.insert(key, value);
                auto mid = std::chrono::high_resolution_clock::now();
                table.remove(key);
                auto end = std::chrono::high_resolution_clock::now();
                total_insert_OA += std::chrono::duration_cast<std::chrono::nanoseconds>(mid - start).count();
                total_remove_OA += std::chrono::duration_cast<std::chrono::nanoseconds>(end - mid).count();
            } catch (const std::exception& e) {
                std::cerr << "[OpenAddressing] Exception: " << e.what() << std::endl;
            }

            // Cuckoo
            HashTableCuckoo tableCuckoo(size);
            for (int i = 0; i < fill_count; ++i) {
                tableCuckoo.insert(keys[i], values[i]);
            }
            tableCuckoo.insert(key, value);
            tableCuckoo.remove(key);
            auto startC = std::chrono::high_resolution_clock::now();
            tableCuckoo.insert(key, value);
            auto midC = std::chrono::high_resolution_clock::now();
            tableCuckoo.remove(key);
            auto endC = std::chrono::high_resolution_clock::now();
            total_insert_Cuckoo += std::chrono::duration_cast<std::chrono::nanoseconds>(midC - startC).count();
            total_remove_Cuckoo += std::chrono::duration_cast<std::chrono::nanoseconds>(endC - midC).count();

            // AVL
            HashTableAVL tableAVL(size);
            for (int i = 0; i < fill_count; ++i) {
                tableAVL.insert(keys[i], values[i]);
            }
            tableAVL.insert(key, value);
            tableAVL.remove(key);
            auto startA = std::chrono::high_resolution_clock::now();
            tableAVL.insert(key, value);
            auto midA = std::chrono::high_resolution_clock::now();
            tableAVL.remove(key);
            auto endA = std::chrono::high_resolution_clock::now();
            total_insert_AVL += std::chrono::duration_cast<std::chrono::nanoseconds>(midA - startA).count();
            total_remove_AVL += std::chrono::duration_cast<std::chrono::nanoseconds>(endA - midA).count();
        }
        // Zapis wyników do pliku CSV
        csv << size << ",OpenAddressing," << (total_insert_OA/N) << "," << (total_remove_OA/N) << "\n";
        csv << size << ",Cuckoo," << (total_insert_Cuckoo/N) << "," << (total_remove_Cuckoo/N) << "\n";
        csv << size << ",AVL," << (total_insert_AVL/N) << "," << (total_remove_AVL/N) << "\n";
    }
    csv.close();
    std::cout << "Badanie złożoności zakończone. Wyniki zapisane w results.csv" << std::endl;
    return 0;
}
