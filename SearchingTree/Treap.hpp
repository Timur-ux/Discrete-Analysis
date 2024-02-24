#ifndef TREAP_HPP_
#define TREAP_HPP_

#include <concepts>
#include <memory>

namespace sorts {

template <typename T>
concept Comparable = requires(T a, T b) {
  a <=> b;
};

template <Comparable TKey, Comparable TPriority>
class Treap {
public:
  Treap(TKey key, TPriority priority);

  using PTreap = std::shared_ptr<Treap>;
  using PTreapPair = std::pair<PTreap, PTreap>;

  std::pair<PTreap, PTreap> split(TKey key);
  void merge(PTreap less, PTreap high);
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
inline typename Treap<TKey, TPriority>::PTreapPair
Treap<TKey, TPriority>::split(TKey key) {
  PTreapPair splittedPair;

  if (key >= key_) {
    splittedPair.first = std::shared_ptr<Treap>(this);
    splittedPair.second = std::shared_ptr<Treap>(nullptr);

    if (right_) {
      auto [leftNew, rightNew] = right_->split(key);
      right_ = leftNew;
      splittedPair.second = rightNew;
    }
  }

  if (key < key_) {
    splittedPair.first = std::shared_ptr<Treap>(nullptr);
    splittedPair.second = std::shared_ptr<Treap>(this);

    if (left_) {
      auto [leftNew, rightNew] = left_->split(key);
      left_ = rightNew;
      splittedPair.first = leftNew;
    }
  }

  return splittedPair;
}

template<Comparable TKey, Comparable TPriority>
inline void
Treap<TKey, TPriority>::merge(PTreap less, PTreap high) {
  if (less->priority_ == high->priority_) {
    ++less->priority_;
  }

  if (less->priority_ == high->priority_) {
    less->priority_;
  }
}

} // !sorts

#endif // !TREAP_HPP_