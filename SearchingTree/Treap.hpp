#ifndef TREAP_HPP_
#define TREAP_HPP_

#include <concepts>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <compare>
#include <optional>

namespace sorts {

template <typename T>
concept Comparable = requires(T a, T b) {
  a <=> b;
  a - 1;
};

template <Comparable TKey, Comparable TPriority>
class Treap {
public:
  Treap(TKey key, TPriority priority = rand());
  ~Treap();

  using PTreap = Treap *;
  using PTreapPair = std::pair<PTreap, PTreap>;

  PTreapPair split(TKey key);
  static PTreap merge(PTreap less, PTreap higher);

  PTreap insert(TKey key, TPriority priority = rand());
  PTreap remove(TKey key);

  PTreap first(); // less to high order
  PTreap last(); // less to high order

  PTreap nthElement(int n);
  static size_t getSize(PTreap treap);
  TKey & getKey();

  void print(std::ostream & os = std::cout, int depth = 0);
  std::optional<PTreap> find(TKey key);
private:
  static void recalc(PTreap treap);

  TKey key_;
  TPriority priority_;

  PTreap left_;
  PTreap right_;

  size_t size_;
};

template<Comparable TKey, Comparable TPriority>
inline
Treap<TKey, TPriority>::Treap(TKey key, TPriority priority)
  : key_(key)
  , priority_(priority)
  , left_(nullptr)
  , right_(nullptr)
  , size_(1) {}

template<Comparable TKey, Comparable TPriority>
inline
Treap<TKey, TPriority>::~Treap() {
  delete left_;
  delete right_;
}

template<Comparable TKey, Comparable TPriority>
inline typename Treap<TKey, TPriority>::PTreapPair
Treap<TKey, TPriority>::split(TKey key) {
  PTreapPair splittedPair = { nullptr, nullptr };

  if (key >= key_) {
    splittedPair.first = this;

    if (right_) {
      auto [leftNew, rightNew] = right_->split(key);
      right_ = leftNew;
      splittedPair.second = rightNew;
    }
  }

  if (key < key_) {
    splittedPair.second = this;

    if (left_) {
      auto [leftNew, rightNew] = left_->split(key);
      left_ = rightNew;
      splittedPair.first = leftNew;
    }
  }

  recalc(splittedPair.first);
  recalc(splittedPair.second);

  return splittedPair;
}

template<Comparable TKey, Comparable TPriority>
inline Treap<TKey, TPriority>::PTreap
Treap<TKey, TPriority>::merge(PTreap less, PTreap higher) {
  if (!less) {
    return higher;
  }

  if (!higher) {
    return less;
  }

  PTreap root;

  if (less->priority_ == higher->priority_) {
    ++less->priority_;
  }

  if (less->priority_ > higher->priority_) {
    root = less;
    root->right_ = merge(root->right_, higher);
  }
  else {
    root = higher;
    root->left_ = merge(less, root->left_);
  }

  recalc(root);

  return root;
}

template<Comparable TKey, Comparable TPriority>
inline Treap<TKey, TPriority>::PTreap
Treap<TKey, TPriority>::insert(TKey key, TPriority priority) {
  auto [less, higher] = split(key);

  PTreap newTreap = new Treap(key, priority);
  PTreap newRoot = merge(merge(less, newTreap), higher);

  return newRoot;
}

template<Comparable TKey, Comparable TPriority>
inline Treap<TKey, TPriority>::PTreap
Treap<TKey, TPriority>::remove(TKey key) {
  auto [temp, higher] = split(key);
  auto [less, current] = temp->split(key - 1);

  delete current;
  PTreap newRoot = merge(less, higher);

  return newRoot;
}

template<Comparable TKey, Comparable TPriority>
inline Treap<TKey, TPriority>::PTreap Treap<TKey, TPriority>::first() {
  PTreap current = this;
  while (current->left_ != nullptr) {
    current = current->left_;
  }

  return current;
}

template<Comparable TKey, Comparable TPriority>
inline Treap<TKey, TPriority>::PTreap Treap<TKey, TPriority>::last() {
  PTreap current = this;
  while (current->right_ != nullptr) {
    current = current->right_;
  }

  return current;
}

template<Comparable TKey, Comparable TPriority>
inline void
Treap<TKey, TPriority>::print(std::ostream & os, int depth) {
  if (right_) {
    right_->print(os, depth + 1);
  }

  for (int _ = 0; _ < depth; ++_) {
    os << '\t';
  }
  os << '<' << key_ << ", " << size_ << '>' << std::endl;

  if (left_) {
    left_->print(os, depth + 1);
  }
}

template<Comparable TKey, Comparable TPriority>
inline std::optional<typename Treap<TKey, TPriority>::PTreap>
Treap<TKey, TPriority>::find(TKey key) {
  if (key_ == key) {
    return { this };
  }

  if (key_ < key) {
    if (right_)
      return right_->find(key);
    return std::nullopt;
  }

  if (left_)
    return left->find(key);
  return std::nullopt;
}

template<Comparable TKey, Comparable TPriority>
inline Treap<TKey, TPriority>::PTreap Treap<TKey, TPriority>::nthElement(int n) {
  if (n > size_) {
    return nullptr;
  }

  if (n == getSize(left_) + 1) {
    return this;
  }

  if (n <= getSize(left_)) {
    return left_->nthElement(n);
  }

  return right_->nthElement(n - getSize(left_) - 1);
}

template<Comparable TKey, Comparable TPriority>
inline void
Treap<TKey, TPriority>::recalc(PTreap treap) {
  if (treap == nullptr) {
    return;
  }

  treap->size_ = 1 + getSize(treap->left_) + getSize(treap->right_);
}

template<Comparable TKey, Comparable TPriority>
inline size_t
Treap<TKey, TPriority>::getSize(PTreap treap) {
  return (treap == nullptr ? 0 : treap->size_);
}

template<Comparable TKey, Comparable TPriority>
inline TKey &
Treap<TKey, TPriority>::getKey() {
  return key_;
}
} // !sorts

#endif // !TREAP_HPP_