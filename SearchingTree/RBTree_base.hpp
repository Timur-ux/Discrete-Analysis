#ifndef RED_BLACK_TREE_BASE_HPP_
#define RED_BLACK_TREE_BASE_HPP_

#include <concepts>
#include <optional>
#include <memory>
#include <iostream>

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
    void insert(TKey key, TVal value);
    void remove(TKey key) {
        // TODO
    };
    std::optional<TVal &> find(TKey key);
    void print(std::ostream & os = std::cout);
private:
    struct Node;
    struct NodeAndParentPair;

    std::shared_ptr<Node> root_;

    void impl_print(std::ostream & os, std::shared_ptr<Node> node, int depth = 0);
    NodeAndParentPair impl_find(TKey key);
    void rotateRight(std::shared_ptr<Node> node);
    void rotateLeft(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getGrandParent(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getUncle(std::shared_ptr<Node> node);

    void insertColorFixup1(std::shared_ptr<Node> node);
    void insertColorFixup2(std::shared_ptr<Node> node);
    void insertColorFixup3(std::shared_ptr<Node> node);
    void insertColorFixup4(std::shared_ptr<Node> node);
    void insertColorFixup5(std::shared_ptr<Node> node);
    struct Node {
        TKey key_;
        TVal value_;

        Color color = Color::red;

        std::shared_ptr<Node> parent = nullptr;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
    };

    struct NodeAndParentPair {
        std::shared_ptr<Node> node;
        std::shared_ptr<Node> parent;
    };

};

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insert(TKey key, TVal value) {
    if (!root_) {
        root_ = std::shared_ptr<Node>{ new Node{
            .key_ = key,
            .value_ = value,
        } };
        insertColorFixup1(root_);
        return;
    }

    auto [node, parent] = impl_find(key);
    if (node) {
        node->value_ = value;
        return;
    }

    std::shared_ptr<Node> insertedNode = std::shared_ptr<Node>(new Node{
            .key_ = key,
            .value_ = value,
            .parent = parent
        });
    if (parent->key_ < key) {
        parent->right = insertedNode;
    }
    else {
        parent->left = insertedNode;
    }

    insertColorFixup1(insertedNode);
}

template<Comparable TKey, typename TVal>
inline std::optional<TVal &>
RBTree_base<TKey, TVal>::find(TKey key) {
    auto [node, _] = impl_find(key);

    if (node)
        return { node->value_ };
    return std::nullopt;
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::print(std::ostream & os) {
    impl_print(os, root_, 0);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::impl_print(std::ostream & os, std::shared_ptr<Node> node, int depth) {
    if (!node) {
        return;
    }

    impl_print(os, node->right, depth + 1);

    for (int _ = 0; _ < depth; ++_) {
        os << '\t';
    }
    os << node->key_ << ", " << (node->color == Color::black ? "B" : "R") << std::endl;

    impl_print(os, node->left, depth + 1);
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
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::rotateRight(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> parent = node->parent;
    std::shared_ptr<Node> grandParent = parent->parent;
    if (parent == root_) {
        root_ = node;
    }

    parent->left = node->right;
    if (node->right) {
        node->right->parent = parent;
    }

    node->right = parent;
    parent->parent = node;

    if (grandParent) {
        if (grandParent->right == parent) {
            grandParent->right = node;
        }
        else {
            grandParent->left = node;
        }
    }
    node->parent = grandParent;
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::rotateLeft(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> parent = node->parent;
    std::shared_ptr<Node> grandParent = parent->parent;
    if (parent == root_) {
        root_ = node;
    }

    parent->right = node->left;
    if (node->left) {
        node->left->parent = parent;
    }

    node->left = parent;
    parent->parent = node;

    if (grandParent) {
        if (grandParent->right == parent) {
            grandParent->right = node;
        }
        else {
            grandParent->left = node;
        }
    }
    node->parent = grandParent;
}
template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::getGrandParent(std::shared_ptr<Node> node) {
    if (node && node->parent) {
        return node->parent->parent;
    }
    return nullptr;
}

template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::getUncle(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> grandParent = getGrandParent(node);
    if (node && grandParent) {
        if (grandParent->left == node->parent) {
            return grandParent->right;
        }
        return grandParent->left;
    }

    return nullptr;
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup1(std::shared_ptr<Node> node) {
    if (!node) {
        return;
    }

    if (node == root_) {
        node->color = Color::black;
        return;
    }
    insertColorFixup2(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup2(std::shared_ptr<Node> node) {
    if (node->parent->color == Color::black) {
        return;
    }
    insertColorFixup3(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup3(std::shared_ptr<Node> node) {
    auto grandParent = getGrandParent(node);
    auto uncle = getUncle(node);
    auto parent = node->parent;

    if (uncle && uncle->color == Color::red) {
        grandParent->color = Color::red;
        uncle->color = Color::black;
        parent->color = Color::black;

        insertColorFixup1(grandParent);
        return;
    }

    insertColorFixup4(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup4(std::shared_ptr<Node> node) {
    auto grandParent = getGrandParent(node);
    auto parent = node->parent;

    auto nextNodeToFixup = node->parent;
    if (grandParent->left == parent && parent->right == node) {
        rotateLeft(node);
        nextNodeToFixup = node;
    }
    else if (grandParent->right == parent && parent->left == node) {
        rotateRight(node);
        nextNodeToFixup = node;
    }

    insertColorFixup5(nextNodeToFixup);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup5(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    if (parent->right == node) {
        rotateLeft(node);
    }
    else {
        rotateRight(node);
    }

    node->color = Color::black;
    parent->color = Color::red;
}
} // ! impl

} // !tree

#endif // !RED_BLACK_TREE_BASE_HPP_