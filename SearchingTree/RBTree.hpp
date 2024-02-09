#ifndef RED_BLACK_TREE_HPP_
#define RED_BLACK_TREE_HPP_
#include "RBTree_base.hpp"

namespace tree {
template <Comparable TKey, typename TVal>
class RBTree {
public:
    void insert(TKey key, TVal val);
    void remove(TKey key);
    std::optional<TVal &> find(TKey key);
private:
    impl::RBTree_base rbTree;
};

} // !tree


#endif // !RED_BLACK_TREE_HPP_