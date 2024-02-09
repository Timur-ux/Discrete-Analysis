#ifndef RED_BLACK_TREE_BASE_HPP_
#define RED_BLACK_TREE_BASE_HPP_

#include <concepts>
#include <optional>
#include <memory>

template <typename T>
concept Comparable = requires(T a, T b) {
    a <=> b;
};

namespace tree {

namespace impl {
enum class Color : bool {
    black = true,
    red = false
};

template <Comparable TKey, typename TVal>
class RBTree_base {
public:
    void insert(TKey key, TVal val);
    void remove(TKey key);
    std::optional<TVal &> find(TKey key);
private:
    struct Node;
    struct NodeAndParentPair;

    std::shared_ptr<Node> root_;

    NodeAndParentPair impl_find(TKey key);
    void rotateRight(std::shared_ptr<Node> node);
    void rotateLeft(std::shared_ptr<Node> node);

    void swapColors(std::shared_ptr<Node> grandParentNode);

    struct Node {
        TKey key_;
        TVal value_;

        Color color = Color::red;

        Node * parent = nullptr;
        Node * left = nullptr;
        Node * right = nullptr;
    };

    struct NodeAndParentPair {
        std::shared_ptr<Node> node;
        std::shared_ptr<Node> parent;
    };

};

template<Comparable TKey, typename TVal>
inline std::optional<TVal &>
RBTree_base<TKey, TVal>::find(TKey key) {
    auto [node, _] = impl_find(key);

    if (node)
        return { node->value_ };
    return std::nullopt;
}

template<Comparable TKey, typename TVal>
inline RBTree_base<TKey, TVal>::NodeAndParentPair
RBTree_base<TKey, TVal>::impl_find(TKey key) {
    if (!root_) {
        return { nullptr, nullptr };
    }

    std::shared_ptr<Node> node = root_;
    while (node->key_ != key) {
        if (node->key_ < key) {
            if (!node->right) {
                return { nullptr, node };
            }
            node = node->right;
        }
        else {
            if (!node->left) {
                return { nullptr, node };
            }
            node = node->left;
        }
    }

    return { node, node->parent };
}
} // ! impl

} // !tree

#endif RED_BLACK_TREE_BASE_HPP_