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
    const int kGenerateCount = 50;
    tree::RBTree<int, int> tree;

    for (int i = 1; i < kGenerateCount; ++i) {
        tree.insert(rand(), i * i);
    }
    tree.print();
    return 0;
}

