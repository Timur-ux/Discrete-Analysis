#ifndef B_TREE_HPP_
#define B_TREE_HPP_

#include "../utils/utils.hpp"
#include <cassert>
#include <cstdint>
#include <vector>

namespace sorts {
template <typename TKey, typename TData> class BTree {
public:
  BTree(uint64_t seed);
  void insert(TKey key, TData data);
  TData * find(TKey key);

private:
  uint64_t seed_;
  struct Node {
    struct Item {
      TKey key_;
      TData data_;
      Node *less = nullptr;
      Node *higher = nullptr;
    };

    Node(uint64_t seed) : seed_(seed) { items_.reserve(2 * seed_ - 1); }

    uint64_t seed_;
    std::vector<Item> items_;
    bool isLeaf() { return items_.size() == 0 || items_[0].less == nullptr; };
    void split();

    const std::function<TKey(Item &)> keyFetcher = [](Item &item) {
      return item.key_;
    };
  };

  Node *root_;
  void insert_impl(Node *node, Node *parent, TKey key, TData data);
};

template <typename TKey, typename TData>
inline BTree<TKey, TData>::BTree(uint64_t seed) : seed_(seed) {
  assert(seed_ >= 2 && "BTree's seed must be >= 2");
  root_ = new Node(seed_);
}

template <typename TKey, typename TData>
inline void BTree<TKey, TData>::insert(TKey key, TData data) {
  if (root_->items_.size() == 2 * seed_ - 1) {
    root_->split();
  }
  if (root_->isLeaf()) {
    auto keyPlace = binSearch<typename Node::Item, TKey>(root_->items_, key,
                                                         root_->keyFetcher);
    root_->items_.insert(keyPlace, {key, data});
    return;
  }
  insert_impl(root_, root_, key, data);
}

template <typename TKey, typename TData>
inline void BTree<TKey, TData>::insert_impl(Node *node, Node *parent, TKey key,
                                            TData data) {
  auto keyPlace =
      binSearch<typename Node::Item, TKey>(node->items_, key, node->keyFetcher);
  if (keyPlace->key_ == key) {
    keyPlace->data_ = data;
    return;
  }
  if (node->items_.size() == 2 * seed_ - 1) {
    node->split();
    auto insertPlace = binSearch<typename Node::Item, TKey>(parent->items_, key,
                                                            node->keyFetcher);
    parent->items_.insert(insertPlace, node->items_[0]);
    (insertPlace - 1)->higher = node->items_[0].less;
    if (key > node->items_[0].key_) {
      insert_impl(node->items_[0].higher, parent, key, data);
    } else {
      insert_impl(node->items_[0].less, parent, key, data);
    }
    delete node;
    return;
  }
  if (node->isLeaf()) {
    node->items_.insert(keyPlace, {.key_ = key, .data_ = data});
    return;
  }

  if (keyPlace == node->items_.end()) {
    insert_impl((--keyPlace)->higher, node, key, data);
  } else if (keyPlace == node->items_.begin()) {
    insert_impl(keyPlace->less, node, key, data);
  } else {
    insert_impl(keyPlace->less, node, key, data);
  }
}

template <typename TKey, typename TData>
void BTree<TKey, TData>::Node::split() {
  assert(items_.size() == 2 * seed_ - 1 &&
         "Key size not equal 2*seed - 1, but splitting is called");
  Node *nodeLeft = new Node(seed_);
  Node *nodeRight = new Node(seed_);

  for (size_t i = 0; i < seed_ - 1; ++i) {
    nodeLeft->items_.push_back(std::move(items_[i]));
  }

  for (size_t i = seed_; i < 2 * seed_ - 1; ++i) {
    nodeRight->items_.push_back(std::move(items_[i]));
  }

  items_[0] = std::move(items_[seed_ - 1]);
  items_[0].less = nodeLeft;
  items_[0].higher = nodeRight;
  items_.erase(std::begin(items_) + 1, std::end(items_));
}

template <typename TKey, typename TData>
TData * BTree<TKey, TData>::find(TKey key) {
  if(root_->items_.empty()) {
    return nullptr;
  }
  Node *currentNode = root_;
  while (currentNode != nullptr) {
    auto currentItem =
        binSearch<typename Node::Item, TKey>(currentNode->items_, key, currentNode->keyFetcher);
    if (currentItem->key_ == key) {
      return &currentItem->data_;
    }

    if (currentItem == std::end(currentNode->items_))
      currentNode = (--currentItem)->higher;
    else
      currentNode = currentItem->less;
  }

  return nullptr;

}
} // namespace sorts

#endif // ! B_TREE_HPP_
