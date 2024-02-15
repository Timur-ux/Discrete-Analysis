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
    void remove(TKey key);
    std::optional<TVal &> find(TKey key);
    void print(std::ostream & os = std::cout);
    bool validate();
private:
    struct Node;
    struct NodeAndParentPair;

    std::shared_ptr<Node> root_;

    int calcBlackHeight(std::shared_ptr<Node>);
    bool impl_validate(std::shared_ptr<Node> node);
    Color getColor(std::shared_ptr<Node> node);
    void impl_print(std::ostream & os, std::shared_ptr<Node> node, int depth = 0);
    NodeAndParentPair impl_find(TKey key);
    void rotateRight(std::shared_ptr<Node> node);
    void rotateLeft(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getGrandParent(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getUncle(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getSibling(std::shared_ptr<Node> node, std::shared_ptr<Node> parent);

    void insertColorFixup1(std::shared_ptr<Node> node);
    void insertColorFixup2(std::shared_ptr<Node> node);
    void insertColorFixup3(std::shared_ptr<Node> node);
    void insertColorFixup4(std::shared_ptr<Node> node);
    void insertColorFixup5(std::shared_ptr<Node> node);

    void removeColorFixup1(std::shared_ptr<Node> node, std::shared_ptr<Node> parent);
    void removeColorFixup2(std::shared_ptr<Node> node, std::shared_ptr<Node> parent);
    void removeColorFixup3(std::shared_ptr<Node> node, std::shared_ptr<Node> parent);
    void removeColorFixup4(std::shared_ptr<Node> node, std::shared_ptr<Node> parent);
    void removeColorFixup5(std::shared_ptr<Node> node, std::shared_ptr<Node> parent);
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
inline void
RBTree_base<TKey, TVal>::remove(TKey key) {
    auto [node, parent] = impl_find(key);

    if (!node) {
        return;
    }

    if (!node->left && !node->right) {
        if (node == root_) {
            root_ = nullptr;
            return;
        }

        if (parent->right == node) {
            parent->right = nullptr;
        }
        else {
            parent->left = nullptr;
        }

        if (node->color == Color::black) {
            removeColorFixup1(nullptr, parent);
        }
        return;
    }

    if (node->right && node->left) {
        auto rightChild = node->right;
        auto deletedNode = rightChild;

        while (deletedNode->left != nullptr) {
            deletedNode = deletedNode->left;
        }

        node->key_ = std::move(deletedNode->key_);
        node->value_ = std::move(deletedNode->value_);

        if (deletedNode == rightChild) {
            deletedNode->parent->right = deletedNode->right;
        }
        else {
            deletedNode->parent->left = deletedNode->right;
        }

        if (deletedNode->right) {
            deletedNode->right->parent = deletedNode->parent;
        }

        if (deletedNode->color == Color::red || getColor(deletedNode->right) == Color::red) {
            if (deletedNode->right)
                deletedNode->right->color = Color::black;
        }
        else {
            removeColorFixup1(deletedNode->right, deletedNode->parent);
        }
        return;
    }

    std::shared_ptr<Node> deletedNode;
    if (node->right) {
        deletedNode = node->right;
    }
    else {
        deletedNode = node->left;
    }

    node->key_ = std::move(deletedNode->key_);
    node->value_ = std::move(deletedNode->value_);

    node->left = deletedNode->left;
    node->right = deletedNode->right;

    if (deletedNode->color == Color::red || node->color == Color::red) {
        node->color = Color::black;
    }
    else {
        removeColorFixup1(node, parent);
    }
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
inline bool RBTree_base<TKey, TVal>::validate() {
    if (!root_) {
        return true;
    }

    if (getColor(root_) == Color::red) {
        std::cerr << "Error: Red root" << std::endl;
    }

    return impl_validate(root_);
}

template<Comparable TKey, typename TVal>
inline int RBTree_base<TKey, TVal>::calcBlackHeight(std::shared_ptr<Node> node) {
    if (!node) {
        return 1;
    }

    int leftBH = calcBlackHeight(node->left);
    int rightBH = calcBlackHeight(node->right);
    if (leftBH != rightBH) {
        std::cerr << "Bad node with key: " << node->key_ << std::endl;
    }

    int BH = std::max(leftBH, rightBH);
    if (getColor(node) == Color::black)
        ++BH;
    return BH;
}

template<Comparable TKey, typename TVal>
inline bool RBTree_base<TKey, TVal>::impl_validate(std::shared_ptr<Node> node) {
    return calcBlackHeight(node->right) == calcBlackHeight(node->left);
}

template<Comparable TKey, typename TVal>
inline Color
RBTree_base<TKey, TVal>::getColor(std::shared_ptr<Node> node) {
    return (node ? node->color : Color::black);
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
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::getSibling(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
    if (node) {
        if (parent->left == node) {
            return parent->right;
        }
        return parent->left;
    }
    else {
        if (parent->right) {
            return parent->right;
        }
        return parent->left;
    }
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
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup1(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
    if (getColor(node) == Color::red) {
        node->color = Color::black;
    }
    else {
        removeColorFixup2(node, parent);
    }
    root_->color = Color::black;
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup2(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
    if (node == root_) {
        return;
    }
    else {
        removeColorFixup3(node, parent);
    }
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup3(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
    std::shared_ptr<Node> sibling = getSibling(node, parent);

    if (!sibling) {
        std::cerr << "Error: realization error -- found no sibling" << std::endl;
    }

    if (sibling->color == Color::red) {
        if (parent->right == sibling) {
            rotateLeft(sibling);
        }
        else {
            rotateRight(sibling);
        }
    }

    removeColorFixup4(node, parent);
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup4(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
    std::shared_ptr<Node> sibling = getSibling(node, parent);

    if (getColor(sibling->left) == Color::black && getColor(sibling->right) == Color::black) {
        sibling->color = Color::red;
        removeColorFixup1(parent, parent->parent);
        return;
    }

    if (sibling == parent->right && getColor(sibling->left) == Color::red) {
        auto siblingLeft = sibling->left;
        rotateRight(sibling->left);
        std::swap(sibling->color, siblingLeft->color);
    }
    else if (sibling == parent->left && getColor(sibling->right) == Color::red) {
        auto siblingRight = sibling->right;
        rotateLeft(sibling->right);
        std::swap(sibling->color, siblingRight->color);
    }

    removeColorFixup5(node, parent);
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup5(std::shared_ptr<Node> node, std::shared_ptr<Node> parent) {
    auto sibling = getSibling(node, parent);

    if (sibling == parent->right) {
        rotateLeft(sibling);
    }
    else {
        rotateRight(sibling);
    }
}
} // ! impl

} // !tree

#endif // !RED_BLACK_TREE_BASE_HPP_