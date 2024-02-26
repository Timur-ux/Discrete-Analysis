#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdint>
#include "SearchingTree/Treap.hpp"

using namespace sorts;

int main() {
  std::vector<uint32_t> data{ 15, 24, 17, 87, 0, 122, 24, 24, 24, 96 };
  auto * treap = new Treap<uint32_t, uint32_t>(50);

  for (auto & elem : data) {
    treap = treap->insert(elem);
  }

  treap->print();
  std::cout << "------------" << std::endl;
  for (int i = 0; i < treap->getSize(treap); ++i) {
    std::cout << i + 1 << ": " << treap->nthElement(i + 1)->getKey() << std::endl;
  }

  return 0;
}