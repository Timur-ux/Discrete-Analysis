#ifndef TREAP_HPP_
#define TREAP_HPP_

#include <concepts>
#include <memory>
#include <iostream>

namespace sorts {

template <typename T>
concept Comparable = requires(T a, T b) {
  a <=> b;
};

template <Comparable TKey, Comparable TPriority>
class Treap {
public:
  Treap(TKey key, TPriority priority);
  ~Treap();

  using PTreap = Treap *;
  using PTreapPair = std::pair<PTreap, PTreap>;

  PTreapPair split(TKey key);
  static PTreap merge(PTreap less, PTreap higher);

  PTreap insert(TKey key, TPriority priority);
  PTreap remove(TKey key);

  void print(std::ostream & os = std::cout, int depth = 0);
private:
  TKey key_;
  TPriority priority_;

  PTreap left_;
  PTreap right_;
};

template<Comparable TKey, Comparable TPriority>
inline
Treap<TKey, TPriority>::Treap(TKey key, TPriority priority)
  : key_(key), priority_(priority), left_(nullptr), right_(nullptr) {}

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
inline void
Treap<TKey, TPriority>::print(std::ostream & os, int depth) {
  if (right_) {
    right_->print(os, depth + 1);
  }

  for (int _ = 0; _ < depth; ++_) {
    os << '\t';
  }
  os << '<' << key_ << ", " << priority_ << '>' << std::endl;

  if (left_) {
    left_->print(os, depth + 1);
  }
}
} // !sorts

#endif // !TREAP_HPP_