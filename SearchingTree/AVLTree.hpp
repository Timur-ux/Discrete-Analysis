#ifndef AVL_TREE_HPP_
#define AVL_TREE_HPP_

#include <concepts>
#include <memory>
#include <optional>
#include <iostream>

template <typename T>
concept Comparable = requires(T a, T b) {
    a <=> b;
};

namespace tree {

namespace impl {
enum Balance {
    badRight = -2,
    right = -1,
    balanced = 0,
    left = 1,
    badLeft = 2
};
} // ! impl

template <Comparable TKey, typename TVal>
class AvlTree {
private:
    struct Node;
    struct NodeAndParentPair;
    std::shared_ptr<Node> root_ = nullptr;

    void rotateLeft(std::shared_ptr<Node> node);
    void rotateRight(std::shared_ptr<Node> node);

    void rebalanceAfterInsertion(std::shared_ptr<Node> node);
    void rebalanceAfterRemove(std::shared_ptr<Node> node);

    NodeAndParentPair impl_find(const TKey key) const;
    void impl_print(std::ostream & os, int depth, std::shared_ptr<Node> node);
    int calc_height(std::shared_ptr<Node> node);
    bool impl_check(std::shared_ptr<Node> node);

    void impl_deleteLeaf(std::shared_ptr<Node> node);
    void impl_deleteFullNode(std::shared_ptr<Node> node);
    void impl_deleteNodeWithOnlyLeftChild(std::shared_ptr<Node> node);
    void impl_deleteNodeWithOnlyRightChild(std::shared_ptr<Node> node);
public:
    void insert(const TKey key, TVal value);
    void remove(const TKey key);
    std::optional<TVal &> find(const TKey key) const;
    void print(std::ostream & os, int depth = 0) {
        impl_print(os, 0, root_);
    }

    bool check_tree() {
        return impl_check(root_);
    }
private:
    struct Node {
        TKey key_;
        TVal value_;

        impl::Balance balance = impl::balanced;
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
AvlTree<TKey, TVal>::rotateLeft(std::shared_ptr<Node> node) {
    if (node == nullptr || node == root_) {
        return;
    }

    std::shared_ptr<Node> parent = node->parent;
    if (parent == root_) {
        root_ = node;
    }

    node->parent = parent->parent;
    if (node->parent) {
        if (node->parent->right == parent) {
            node->parent->right = node;
        }
        else {
            node->parent->left = node;
        }
    }
    parent->parent = node;

    parent->right = node->left;
    node->left = parent;
    if (parent->right) {
        parent->right->parent = parent;
    }
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::rotateRight(std::shared_ptr<Node> node) {
    if (node == nullptr || node == root_) {
        return;
    }

    std::shared_ptr<Node> parent = node->parent;
    if (parent == root_) {
        root_ = node;
    }

    node->parent = parent->parent;
    if (node->parent) {
        if (node->parent->right == parent) {
            node->parent->right = node;
        }
        else {
            node->parent->left = node;
        }
    }
    parent->parent = node;

    parent->left = node->right;
    node->right = parent;
    if (parent->left) {
        parent->left->parent = parent;
    }
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::rebalanceAfterInsertion(std::shared_ptr<Node> node) {
    if (node->balance == impl::balanced) {
        return;
    }

    std::shared_ptr<Node> parent = node->parent;
    if (node->balance == impl::left || node->balance == impl::right) {
        if (node == root_) {
            return;
        }

        if (parent->right == node) {
            parent->balance = static_cast<impl::Balance>(parent->balance - 1);
        }
        else if (parent->left == node) {
            parent->balance = static_cast<impl::Balance>(parent->balance + 1);;
        }

        rebalanceAfterInsertion(parent);
        return;
    }
    else if (node->balance == impl::badRight) {
        std::shared_ptr<Node> rightChild = node->right;
        if (rightChild->balance == impl::right) {
            rotateLeft(rightChild);
            node->balance = impl::balanced;
            rightChild->balance = impl::balanced;
        }
        else {
            std::shared_ptr<Node> rightLeftChild = rightChild->left;
            rotateRight(rightLeftChild);
            rotateLeft(rightLeftChild);

            node->balance = impl::balanced;
            rightChild->balance = impl::balanced;

            if (rightLeftChild->balance == impl::left) {
                rightChild->balance = impl::right;
            }
            else if (rightLeftChild->balance == impl::right) {
                node->balance = impl::left;
            }
            rightLeftChild->balance = impl::balanced;
        }
        return;
    }
    else if (node->balance == impl::badLeft) {
        std::shared_ptr<Node> leftChild = node->left;
        if (leftChild->balance == impl::left) {
            rotateRight(leftChild);
            node->balance = impl::balanced;
            leftChild->balance = impl::balanced;
        }
        else {
            std::shared_ptr<Node> leftRightChild = leftChild->right;
            rotateLeft(leftRightChild);
            rotateRight(leftRightChild);

            node->balance = impl::balanced;
            leftChild->balance = impl::balanced;

            if (leftRightChild->balance == impl::left) {
                node->balance = impl::right;
            }
            else if (leftRightChild->balance == impl::right) {
                leftChild->balance = impl::left;
            }
            leftRightChild->balance = impl::balanced;
        }
        return;
    }
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::rebalanceAfterRemove(std::shared_ptr<Node> node) {
    if (node->balance == impl::left || node->balance == impl::right) {
        return;
    }

    if (node->balance == impl::balanced) {
        if (node == root_) {
            return;
        }

        std::shared_ptr<Node> parent = node->parent;
        if (parent->right == node) {
            parent->balance = static_cast<impl::Balance>(parent->balance + 1);
        }
        else {
            parent->balance = static_cast<impl::Balance>(parent->balance - 1);
        }

        rebalanceAfterRemove(parent);
        return;
    }

    if (node->balance == impl::badLeft) {
        std::shared_ptr<Node> leftChild = node->left;

        if (leftChild->balance == impl::left) {
            rotateRight(leftChild);

            node->balance = impl::balanced;
            leftChild->balance = impl::balanced;

            rebalanceAfterRemove(leftChild);
            return;
        }

        if (leftChild->balance == impl::balanced) {
            rotateRight(leftChild);

            node->balance = impl::left;
            leftChild->balance = impl::right;

            return;
        }

        // leftChild->balance == impl::right
        std::shared_ptr<Node> leftRightChild = leftChild->right;
        rotateLeft(leftRightChild);
        rotateRight(leftRightChild);

        node->balance = impl::balanced;
        leftChild->balance = impl::balanced;

        if (leftRightChild->balance == impl::left) {
            node->balance = impl::right;
        }
        if (leftRightChild->balance == impl::right) {
            leftChild->balance = impl::left;
        }

        leftRightChild->balance = impl::balanced;
        rebalanceAfterRemove(leftRightChild);
        return;
    }

    if (node->balance == impl::badRight) {
        std::shared_ptr<Node> rightChild = node->right;

        if (rightChild->balance == impl::right) {
            rotateLeft(rightChild);

            node->balance = impl::balanced;
            rightChild->balance = impl::balanced;

            rebalanceAfterRemove(rightChild);
            return;
        }

        if (rightChild->balance == impl::balanced) {
            rotateLeft(rightChild);

            node->balance = impl::right;
            rightChild->balance = impl::left;

            return;
        }

        // rightChild->balance == impl::left
        std::shared_ptr<Node> rightLeftChild = rightChild->left;
        rotateRight(rightLeftChild);
        rotateLeft(rightLeftChild);

        node->balance = impl::balanced;
        rightChild->balance = impl::balanced;

        if (rightLeftChild->balance == impl::left) {
            rightChild->balance = impl::right;
        }

        if (rightLeftChild->balance == impl::right) {
            node->balance = impl::left;
        }

        rightLeftChild->balance = impl::balanced;
        rebalanceAfterRemove(rightLeftChild);
        return;
    }
}

template<Comparable TKey, typename TVal>
inline AvlTree<TKey, TVal>::NodeAndParentPair
AvlTree<TKey, TVal>::impl_find(const TKey key) const {
    if (root_ == nullptr) {
        return { nullptr, nullptr };
    }

    std::shared_ptr<Node> currentNode = root_;
    while (currentNode->key_ != key) {
        if (currentNode->key_ > key) {
            if (!currentNode->left) {
                return { nullptr, currentNode };
            }
            currentNode = currentNode->left;
        }
        else {
            if (!currentNode->right) {
                return { nullptr, currentNode };
            }
            currentNode = currentNode->right;
        }
    }

    return { currentNode, currentNode->parent };
}

template<Comparable TKey, typename TVal>
inline void AvlTree<TKey, TVal>::impl_print(std::ostream & os, int depth, std::shared_ptr<Node> node) {
    if (node == nullptr) {
        return;
    }

    impl_print(os, depth + 1, node->right);
    for (int _ = 0; _ < depth; ++_) {
        os << '\t';
    }
    os << node->key_ << ", " << static_cast<int>(node->balance) << std::endl;
    impl_print(os, depth + 1, node->left);
}

template<Comparable TKey, typename TVal>
inline int AvlTree<TKey, TVal>::calc_height(std::shared_ptr<Node> node) {
    if (node == nullptr) {
        return 0;
    }
    return std::max(calc_height(node->left), calc_height(node->right)) + 1;
}

template<Comparable TKey, typename TVal>
inline bool AvlTree<TKey, TVal>::impl_check(std::shared_ptr<Node> node) {
    if (node == nullptr) {
        return true;
    }
    int h1 = calc_height(node->right);
    int h2 = calc_height(node->left);
    if (std::abs(h1 - h2) > 1) {
        std::cout << "Bad node with key: " << node->key_ << std::endl;
        return false;
    }
    return impl_check(node->left) && impl_check(node->right);
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::impl_deleteLeaf(std::shared_ptr<Node> node) {
    if (node == root_) {
        root_ = nullptr;
        return;
    }

    std::shared_ptr<Node> parent = node->parent;
    if (parent->right == node) {
        parent->right = nullptr;
        parent->balance = static_cast<impl::Balance>(parent->balance + 1);
    }
    else {
        parent->left = nullptr;
        parent->balance = static_cast<impl::Balance>(parent->balance - 1);
    }

    rebalanceAfterRemove(parent);
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::impl_deleteFullNode(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> rightChild = node->right;
    std::shared_ptr<Node> rightLeftChild = rightChild->left;

    if (rightLeftChild) {
        std::shared_ptr<Node> nodeToDelete = rightLeftChild;
        while (nodeToDelete->left) {
            nodeToDelete = nodeToDelete->left;
        }

        node->key_ = std::move(nodeToDelete->key_);
        node->value_ = std::move(nodeToDelete->value_);

        nodeToDelete->parent->left = nodeToDelete->right;
        if (nodeToDelete->right) nodeToDelete->right->parent = nodeToDelete->parent;

        nodeToDelete->parent->balance = static_cast<impl::Balance>(nodeToDelete->parent->balance - 1);
        rebalanceAfterRemove(nodeToDelete->parent);
    }
    else {
        node->key_ = std::move(rightChild->key_);
        node->value_ = std::move(rightChild->value_);

        node->right = rightChild->right;
        if (node->right) node->right->parent = node;

        node->balance = static_cast<impl::Balance>(node->balance + 1);
        rebalanceAfterRemove(node);
    }
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::impl_deleteNodeWithOnlyLeftChild(std::shared_ptr<Node> node) {
    if (node == root_) {
        root_ = node->left;
        root_->parent = nullptr;
        return;
    }

    std::shared_ptr<Node> parent = node->parent;
    std::shared_ptr<Node> leftChild = node->left;

    if (parent->right == node) {
        parent->right = leftChild;

        parent->balance = static_cast<impl::Balance>(parent->balance + 1);
    }
    else {
        parent->left = leftChild;

        parent->balance = static_cast<impl::Balance>(parent->balance - 1);
    }
    leftChild->parent = parent;

    rebalanceAfterRemove(parent);
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::impl_deleteNodeWithOnlyRightChild(std::shared_ptr<Node> node) {
    if (node == root_) {
        root_ = node->right;
        root_->parent = nullptr;
        return;
    }

    std::shared_ptr<Node> parent = node->parent;
    std::shared_ptr<Node> rightChild = node->right;

    if (parent->right == node) {
        parent->right = rightChild;

        parent->balance = static_cast<impl::Balance>(parent->balance + 1);
    }
    else {
        parent->left = rightChild;

        parent->balance = static_cast<impl::Balance>(parent->balance - 1);
    }
    rightChild->parent = parent;

    rebalanceAfterRemove(parent);

}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::insert(const TKey key, TVal value) {
    if (root_ == nullptr) {
        root_ = std::shared_ptr<Node>(new Node{
              .key_ = key
            , .value_ = value
        });
        return;
    }

    auto [node, parent] = impl_find(key);
    if (node != nullptr) { // means that the key already in tree
        node->value_ = value;
        return;
    }

    if (parent->key_ > key) {
        parent->left = std::shared_ptr<Node>(new Node{
              .key_ = key
            , .value_ = value
            , .parent = parent
        });
        parent->balance = static_cast<impl::Balance>(parent->balance + 1);
        rebalanceAfterInsertion(parent);
        return;
    }

    if (parent->key_ < key) {
        parent->right = std::shared_ptr<Node>(new Node{
              .key_ = key
            , .value_ = value
            , .parent = parent
        });
        parent->balance = static_cast<impl::Balance>(parent->balance - 1);
        rebalanceAfterInsertion(parent);
        return;
    }
}

template<Comparable TKey, typename TVal>
inline void
AvlTree<TKey, TVal>::remove(const TKey key) {
    auto [node, parent] = impl_find(key);

    if (!node) {
        std::cerr << "This key: " << key << " doesn't in the tree" << std::endl;
        return;
    }

    if (!node->left && !node->right) {
        impl_deleteLeaf(node);
    }
    else if (node->left && node->right) {
        impl_deleteFullNode(node);
    }
    else if (node->left) {
        impl_deleteNodeWithOnlyLeftChild(node);
    }
    else if (node->right) {
        impl_deleteNodeWithOnlyRightChild(node);
    }
    else {
        std::cerr << "Undefined removing case" << std::endl;
    }
}

template<Comparable TKey, typename TVal>
inline std::optional<TVal &>
AvlTree<TKey, TVal>::find(TKey key) const {
    auto [node, _] = impl_find(key);
    if (node) {
        return { node->value_ };
    }
    return std::nullopt;
}
} // ! tree

#endif // !AVL_TREE_HPP_