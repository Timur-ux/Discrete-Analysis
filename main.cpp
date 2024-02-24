#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstdint>
#include "SearchingTree/Treap.hpp"

using namespace sorts;

int main() {
  std::vector<uint32_t> data{ 15, 24, 17, 87, 0, 122, 24, 24, 24, 96 };
  auto * treap = new Treap<uint32_t, uint32_t>(50, rand());

  for (auto & elem : data) {
    treap = treap->insert(elem, rand());
  }

  treap->print();
  std::cout << "------------" << std::endl;
  treap = treap->remove(24);
  treap->print();

  return 0;
}