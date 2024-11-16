#include <iostream>
#include <cstdlib>
#include <vector>
#include "Sorts/CountingSort.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    std::vector<Item> data;

    unsigned short key;
    unsigned long long value;
    while (std::cin >> key >> value) {
        data.emplace_back(Item{ key, value });
    }

    sorts::CountingSort sorter;
    sorter.sort(data);

    for (auto & item : data) {
        std::cout << item.key << '\t' << item.value << '\n';
    }

    return 0;
}