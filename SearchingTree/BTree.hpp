#ifndef B_TREE_HPP_
#define B_TREE_HPP_

#include "Treap.hpp"
#include <compare>
#include <optional>

namespace sorts {

template <uint64_t Seed = 2, Comparable TKey, typename TData>
  requires (Seed >= 2)
class BTree {
public:
  BTree() = default;

  std::optional<TData &> find(const TKey key) const;
  void insert(const TKey key, TData data);
  void remove(const TKey key);
private:
  class Node;
  Node * root_ = nullptr;

  class Node {
  public:
    Node(TKey key, TData data);

    std::optional<TData &> find(const TKey key);

    using SplittedNode = std::pair<Node *, Node *>;
    SplittedNode split(const TKey key);
    static Node * merge(const Node * less, Node * higher);
  private:
    struct Key;
    using Keys = Treap<Key, int>;

    Keys * keys_;

    struct Key {
      TKey key;
      TData data;
      Node * less = nullptr;
      Node * higher = nullptr;

      std::strong_ordering operator<=>(Node & other) const;
    };
  };
};

template<uint64_t Seed, Comparable TKey, typename TData>
inline std::optional<TData &>
BTree<Seed, TKey, TData>::find(const TKey key) const {
  if (root_ == nullptr) {
    return std::nullopt;
  }

  return root_->find(key);
}

template<uint64_t Seed, Comparable TKey, typename TData>
inline std::optional<TData &>
BTree<Seed, TKey, TData>::Node::find(const TKey key) {
  std::optional<TData &> data = keys_->find(key);

  if (data) {
    return data;
  }

  auto [less, higher] = keys_->split(key);
  Key * nextKey = higher->first();
  Node * nextNode;

  if (nextKey != nullptr) {
    nextNode = nextKey->less;
  }
  else {
    nextKey = less->last();
    nextNode = nextKey->higher;
  }

  keys_ = Keys::merge(less, higher);

  return nextNode->find(key);
}
} // ! sorts

#endif // ! B_TREE_HPP_