#include <iostream>
#include <cstdlib>
#include "SearchingTree/RBTree.hpp"
#include <vector>

template <typename T>
void asBytes(T n) {
    size_t size = sizeof(T);
    int mask = 15;
    for (int i = size - 1; i >= 0; --i) {
        std::cout << "Byte " << i + 1 << ": " << ((n >> i * 8) & mask) << std::endl;
    }
}

template <typename T>
void asBits(T n) {
    size_t size = sizeof(T);
    int mask = 1;

    for (int i = size * 8 - 1; i >= 0; --i) {
        std::cout << ((n >> i) & mask);
    }
}

int main() {
    const int kGenerateCount = 20;
    std::vector<int> keys;
    tree::RBTree<int, int> tree;

    std::cout << "-----------Inserting------------" << std::endl;
    for (int i = 0; i < kGenerateCount; ++i) {
        int key = rand();
        tree.insert(key, i);
        keys.push_back(key);
        if (!tree.validate()) {
            std::cout << "Bad tree" << std::endl;
            break;
        }
    }
    tree.print();
    std::cout << "-----------Deleting------------" << std::endl;
    for (int i = 0; i < kGenerateCount / 2; ++i) {
        int index = rand() % keys.size();
        int key = keys[index];
        std::cout << "The key is deleted: " << key << std::endl;
        tree.remove(key);
        keys.erase(std::begin(keys) + index);

        if (!tree.validate()) {
            std::cout << "Bad tree" << std::endl;
            break;
        }
        tree.print();
        std::cout << "Deleted key: " << key << std::endl;
        std::cout << "-----------------------" << std::endl;

    }
    tree.print();
    return 0;
}

