#ifndef B_TREE_HPP_
#define B_TREE_HPP_

#include "Treap.hpp"

namespace sorts {

template <Comparable TKey>
class BTree {
public:
  BTree();

private:
  struct BKey {
    TKey key;
    BTree * less = nullptr;
    BTree * higher = nullptr;

    bool operator<=>(BKey & other);
  };


};

} // ! sorts

#endif // ! B_TREE_HPP_