#ifndef B_TREE_HPP_
#define B_TREE_HPP_

#include "../utils/utils.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace sorts {
template <typename TKey, typename TData, uint64_t Seed = 50> class BTree {
public:
  BTree();
  void insert(TKey key, TData data);

private:
  struct Node {
    struct Item {
      TKey key_;
      TData data_;
    };

    std::vector<Item> items_;
    std::vector<Node *> childs_;
    bool isLeaf();
    size_t size();
    Node *split();
  };

  Node *root_;
};

template <typename TKey, typename TData, uint64_t Seed>
inline BTree<TKey, TData, Seed>::BTree() {
  root_ = new Node;
  root_->items_.reserve(Seed);
  root_->childs_.reserve(Seed);
}

template <typename TKey, typename TData, uint64_t Seed>
inline void BTree<TKey, TData, Seed>::insert(TKey key, TData data) {
  if (root_->isLeaf() && root_->size() < 2 * Seed - 1) {
    auto keyToInsert =
        binSearch(root_->keys, key, [](Node::Item &item) { return item.key_; });
    root_->items_.insert(keyToInsert, {.key_ = key, .data_ = data});

    return;
  } else if (root_->size() == 2 * Seed - 1) {
  }
}

template <typename TKey, typename TData, uint64_t Seed>
inline BTree<TKey, TData, Seed>::Node * BTree<TKey, TData, Seed>::Node::split() {
//TODO
}
} // namespace sorts

#endif // ! B_TREE_HPP_
