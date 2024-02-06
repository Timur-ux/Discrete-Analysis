#include <iostream>
#include "Sorts/BucketSort.hpp"
#include <vector>
#include <cstdlib>


int main() {
    const int kGenerateCount = 10;

    std::vector<double> values;
    sorts::BucketSort<double> sorter;
    sorter.setSegmentationSize(10);

    for (int i = 0; i < kGenerateCount; ++i) {
        double value = (double)(rand()) / (rand() % 1000 + 1);
        values.push_back(value);
    }

    std::cout << "<<<: ";
    for (auto & value : values) {
        std::cout << value << ' ';
    }
    std::cout << std::endl;

    sorter.sort(values);

    std::cout << ">>>: ";
    for (auto & value : values) {
        std::cout << value << ' ';
    }
    std::cout << std::endl;

    return 0;
}