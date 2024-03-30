#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
// #include "SearchingTree/BTree.hpp"
#include "utils/utils.hpp"
#include <cstdlib>

int main() {
  const int n = 10;
  std::vector<int> data;
  for (size_t i = 0; i < n; ++i) {
    data.emplace_back(rand()%1000);
  }

  std::sort(data.begin(), data.end()); 

  for (auto &item : data) {
    std::cout << item << ' ';
  }
  std::cout << std::endl;

  for (auto &item : data) {
    std::cout << item << ' '
              << *binSearch<int, int>(data, item, [](int item) { return item; })
              << std::endl;
  }

  return 0;
}
