#ifndef RED_BLACK_TREE_HPP_
#define RED_BLACK_TREE_HPP_
#include "RBTree_base.hpp"

namespace tree {
template <Comparable TKey, typename TVal>
class RBTree {
public:
    void insert(TKey key, TVal value);
    void remove(TKey key);
    std::optional<TVal &> find(TKey key);
    void print(std::ostream & os = std::cout);
private:
    impl::RBTree_base<TKey, TVal> rbTree_base_;
};

template<Comparable TKey, typename TVal>
inline void
RBTree<TKey, TVal>::insert(TKey key, TVal value) {
    rbTree_base_.insert(key, value);
}
template<Comparable TKey, typename TVal>
inline void
RBTree<TKey, TVal>::remove(TKey key) {
    rbTree_base_.remove(key);
}

template<Comparable TKey, typename TVal>
inline std::optional<TVal &>
RBTree<TKey, TVal>::find(TKey key) {
    return rbTree_base_.find(key);
}

template<Comparable TKey, typename TVal>
inline void
RBTree<TKey, TVal>::print(std::ostream & os) {
    rbTree_base_.print(os);
}

} // !tree


#endif // !RED_BLACK_TREE_HPP_