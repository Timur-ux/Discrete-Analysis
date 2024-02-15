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
    RBTree_base() {
        nil_ = std::shared_ptr<Node>(new Node{
            .color = Color::black
        });

        root_ = nil_;
    }

    void insert(TKey key, TVal value);
    void remove(TKey key);
    std::optional<TVal &> find(TKey key);
    void print(std::ostream & os = std::cout);
    bool validate();
private:
    struct Node;
    struct NodeAndParentPair;

    std::shared_ptr<Node> root_;
    std::shared_ptr<Node> nil_;

    int calcBlackHeight(std::shared_ptr<Node>);
    bool impl_validate(std::shared_ptr<Node> node);
    void impl_print(std::ostream & os, std::shared_ptr<Node> node, int depth = 0);

    std::shared_ptr<Node> impl_find(TKey key);
    void rotateRight(std::shared_ptr<Node> node);
    void rotateLeft(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getGrandParent(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getUncle(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getSibling(std::shared_ptr<Node> node);

    void insertColorFixup1(std::shared_ptr<Node> node);
    void insertColorFixup2(std::shared_ptr<Node> node);
    void insertColorFixup3(std::shared_ptr<Node> node);
    void insertColorFixup4(std::shared_ptr<Node> node);
    void insertColorFixup5(std::shared_ptr<Node> node);

    void removeColorFixup1(std::shared_ptr<Node> node);
    void removeColorFixup2(std::shared_ptr<Node> node);
    void removeColorFixup3(std::shared_ptr<Node> node);
    void removeColorFixup4(std::shared_ptr<Node> node);
    void removeColorFixup5(std::shared_ptr<Node> node);
    void removeColorFixup6(std::shared_ptr<Node> node);

    std::shared_ptr<Node> createNode(TKey key, TVal value, std::shared_ptr<Node> parent);
    struct Node {
        std::optional<TKey> key_ = std::nullopt;
        std::optional<TVal> value_ = std::nullopt;

        Color color = Color::red;

        std::shared_ptr<Node> parent = nullptr;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
    };
};

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insert(TKey key, TVal value) {
    if (root_ == nil_) {
        root_ = createNode(key, value, nil_);
        insertColorFixup1(root_);
        return;
    }

    auto node = impl_find(key);
    if (node != nil_) {
        node->value_ = value;
        return;
    }

    auto parent = node->parent;
    std::shared_ptr<Node> insertedNode = createNode(
        key
        , value
        , parent
    );

    if (parent->key_ < key)
        parent->right = insertedNode;
    else
        parent->left = insertedNode;

    insertColorFixup1(insertedNode);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::remove(TKey key) {
    auto node = impl_find(key);

    if (node == nil_) {
        return;
    }

    auto parent = node->parent;
    if (node->left == nil_ && node->right == nil_) {
        if (node == root_) {
            root_ = nil_;
            return;
        }

        if (parent->right == node)
            parent->right = nil_;
        else
            parent->left = nil_;

        nil_->parent = parent;
        if (node->color == Color::black)
            removeColorFixup1(nil_);
        return;
    }

    if (node->right != nil_ && node->left != nil_) {
        auto rightChild = node->right;
        auto deletedNode = rightChild;

        while (deletedNode->left != nil_) {
            deletedNode = deletedNode->left;
        }

        node->key_ = std::move(deletedNode->key_);
        node->value_ = std::move(deletedNode->value_);

        if (deletedNode == rightChild)
            node->right = rightChild->right;
        else
            deletedNode->parent->left = deletedNode->right;

        deletedNode->right->parent = deletedNode->parent;

        if (deletedNode->color == Color::red)
            return;
        else if (deletedNode->right->color == Color::red) {
            deletedNode->right->color = Color::black;
            return;
        }
        else
            removeColorFixup1(deletedNode->right);
        return;
    }

    std::shared_ptr<Node> deletedNode;
    if (node->right != nil_) {
        deletedNode = node->right;
    }
    else {
        deletedNode = node->left;
    }

    node->key_ = std::move(deletedNode->key_);
    node->value_ = std::move(deletedNode->value_);

    node->left = deletedNode->left;
    node->left->parent = node;

    node->right = deletedNode->right;
    node->right->parent = node;

    if (deletedNode->color == Color::red || node->color == Color::red) {
        node->color = Color::black;
    }
    else {
        removeColorFixup1(node);
    }
}

template<Comparable TKey, typename TVal>
inline std::optional<TVal &>
RBTree_base<TKey, TVal>::find(TKey key) {
    auto node = impl_find(key);

    return node->value_;
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

    if (root_->color == Color::red) {
        std::cerr << "Error: Red root" << std::endl;
    }

    return impl_validate(root_);
}

template<Comparable TKey, typename TVal>
inline int RBTree_base<TKey, TVal>::calcBlackHeight(std::shared_ptr<Node> node) {
    if (node == nil_) {
        return 1;
    }

    int leftBH = calcBlackHeight(node->left);
    int rightBH = calcBlackHeight(node->right);
    if (leftBH != rightBH) {
        std::cerr << "Bad node with key: " << *node->key_ << std::endl;
    }

    int BH = std::max(leftBH, rightBH);
    if (node->color == Color::black)
        ++BH;
    return BH;
}

template<Comparable TKey, typename TVal>
inline bool RBTree_base<TKey, TVal>::impl_validate(std::shared_ptr<Node> node) {
    return calcBlackHeight(node->right) == calcBlackHeight(node->left);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::impl_print(std::ostream & os, std::shared_ptr<Node> node, int depth) {
    if (node == nil_) {
        return;
    }

    impl_print(os, node->right, depth + 1);

    for (int _ = 0; _ < depth; ++_) {
        os << '\t';
    }
    os << *node->key_ << ", " << (node->color == Color::black ? "B" : "R") << std::endl;

    impl_print(os, node->left, depth + 1);
}

template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::impl_find(TKey key) {
    if (root_ == nil_) {
        return nil_;
    }

    std::shared_ptr<Node> node = root_;
    while (node->key_ != key) {
        if (node->key_ < key) {
            if (node->right == nil_) {
                nil_->parent = node;
                return nil_;
            }
            node = node->right;
        }
        else {
            if (node->left == nil_) {
                nil_->parent = node;
                return nil_;
            }
            node = node->left;
        }
    }
    return node;
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::rotateRight(std::shared_ptr<Node> node) {
    if (!node || node->left == nil_) {
        std::cerr << "Trying to rotate nullptr or nil node" << std::endl;
        return;
    }

    auto parent = node->parent;
    auto pivot = node->left;

    node->left = pivot->right;
    if (pivot->right != nil_)
        pivot->right->parent = node;

    pivot->right = node;
    node->parent = pivot;

    pivot->parent = parent;
    if (node == root_)
        root_ = pivot;
    else {
        if (parent->right == node)
            parent->right = pivot;
        else
            parent->left = pivot;
    }
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::rotateLeft(std::shared_ptr<Node> node) {
    if (!node || node->right == nil_) {
        std::cerr << "Trying to rotate nullptr or nil node" << std::endl;
        return;
    }

    auto parent = node->parent;
    auto pivot = node->right;

    node->right = pivot->left;
    if (pivot->left != nil_)
        pivot->left->parent = node;

    pivot->left = node;
    node->parent = pivot;

    pivot->parent = parent;
    if (node == root_)
        root_ = pivot;
    else {
        if (parent->right == node)
            parent->right = pivot;
        else
            parent->left = pivot;
    }
}

template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::getGrandParent(std::shared_ptr<Node> node) {
    if (node && node->parent)
        return node->parent->parent;
    return nullptr;
}

template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::getUncle(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> grandParent = getGrandParent(node);
    if (node && grandParent) {
        if (grandParent->left == node->parent)
            return grandParent->right;
        return grandParent->left;
    }
    return nullptr;
}

template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::getSibling(std::shared_ptr<Node> node) {
    if (node->parent->right == node)
        return node->parent->left;
    return node->parent->right;
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup1(std::shared_ptr<Node> node) {
    if (!node)
        return;

    if (node == root_) {
        node->color = Color::black;
        return;
    }
    insertColorFixup2(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup2(std::shared_ptr<Node> node) {
    if (node->parent->color == Color::black)
        return;
    insertColorFixup3(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup3(std::shared_ptr<Node> node) {
    auto grandParent = getGrandParent(node);
    auto uncle = getUncle(node);
    auto parent = node->parent;

    if (uncle->color == Color::red) {
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

    auto nextNodeToFixup = parent;
    if (grandParent->left == parent && parent->right == node) {
        rotateLeft(parent);
        nextNodeToFixup = node;
    }
    else if (grandParent->right == parent && parent->left == node) {
        rotateRight(parent);
        nextNodeToFixup = node;
    }

    insertColorFixup5(nextNodeToFixup);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::insertColorFixup5(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    if (parent->right == node)
        rotateLeft(parent);
    else
        rotateRight(parent);

    node->color = Color::black;
    parent->color = Color::red;
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup1(std::shared_ptr<Node> node) {
    if (node != root_)
        removeColorFixup2(node);
    root_->color = Color::black;
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup2(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    auto sibling = getSibling(node);

    if (sibling->color == Color::red) {
        if (parent->right == sibling)
            rotateLeft(parent);
        else
            rotateRight(parent);
        std::swap(parent->color, sibling->color);
    }

    removeColorFixup3(node);
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup3(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    auto sibling = getSibling(node);

    if (
        parent->color == Color::black &&
        sibling->left->color == Color::black &&
        sibling->right->color == Color::black
    ) {
        sibling->color = Color::red;
        removeColorFixup1(parent);
    }
    else
        removeColorFixup4(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup4(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    auto sibling = getSibling(node);

    if (
        parent->color == Color::red &&
        sibling->left->color == Color::black &&
        sibling->right->color == Color::black
    ) {
        std::swap(parent->color, sibling->color);
    }
    else
        removeColorFixup5(node);
}

template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup5(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    auto sibling = getSibling(node);

    if (
        parent->left == node &&
        sibling->right->color == Color::black
    ) {
        std::swap(sibling->color, sibling->left->color);
        rotateRight(sibling);
    }
    else if (
        parent->right == node &&
        sibling->left->color == Color::black
    ) {
        std::swap(sibling->color, sibling->right->color);
        rotateLeft(sibling);
    }

    removeColorFixup6(node);
}
template<Comparable TKey, typename TVal>
inline void
RBTree_base<TKey, TVal>::removeColorFixup6(std::shared_ptr<Node> node) {
    auto parent = node->parent;
    auto sibling = getSibling(node);

    std::swap(sibling->color, parent->color);

    if (parent->left == sibling) {
        sibling->left->color = Color::black;
        rotateRight(parent);
    }
    else {
        sibling->right->color = Color::black;
        rotateLeft(parent);
    }
}

template<Comparable TKey, typename TVal>
inline std::shared_ptr<typename RBTree_base<TKey, TVal>::Node>
RBTree_base<TKey, TVal>::createNode(TKey key, TVal value, std::shared_ptr<Node> parent) {
    return std::shared_ptr<Node>{new Node{
        .key_ = key,
        .value_ = value,
        .parent = parent,
        .left = nil_,
        .right = nil_
    }};
}
} // ! impl

} // !tree

#endif // !RED_BLACK_TREE_BASE_HPP_