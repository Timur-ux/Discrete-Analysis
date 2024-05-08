#include <gtest/gtest.h>
#include "BTree.hpp"

TEST(BTree, insert) {
  sorts::BTree<int, int> btree(3);
  for(int i = 0; i < 25; ++i) {
    btree.insert(i, i);
  }
  
  for(int _ = 0; _ < 300; ++_) {
    int key = rand() % 50;
    ASSERT_TRUE((key < 25) == (btree.find(key) != nullptr));
  }
}

