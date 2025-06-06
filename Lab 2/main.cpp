#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

class AVLTree {
private:
  struct Node {
    Node *left_;
    Node *right_;
    Node *parent_;
    int8_t balance_ = 0; // | right | - | left |

    std::string key_;
    uint64_t data_;

    Node(std::string key, uint64_t data, Node *parent, Node *left, Node *right)
        : key_(key), data_(data), parent_(parent), left_(left), right_(right) {
      if (left_)
        left_->parent_ = this;
      if (right_)
        right_->parent_ = this;
    }
  };

  Node *root_;
  Node *nil_;

  void rotate_left(Node *node) {
    Node *pivot = node->right_;
    if (node == root_)
      root_ = pivot;

    pivot->parent_ = node->parent_;
    if (node->parent_->left_ == node)
      node->parent_->left_ = pivot;
    else
      node->parent_->right_ = pivot;
    node->parent_ = pivot;

    node->right_ = pivot->left_;
    node->right_->parent_ = node;
    pivot->left_ = node;
  }

  void rotate_right(Node *node) {
    Node *pivot = node->left_;
    if (node == root_)
      root_ = pivot;

    pivot->parent_ = node->parent_;
    if (node->parent_->left_ == node)
      node->parent_->left_ = pivot;
    else
      node->parent_->right_ = pivot;
    node->parent_ = pivot;

    node->left_ = pivot->right_;
    node->left_->parent_ = node;
    pivot->right_ = node;
  }

  void balance_insert(Node *node) {
    if (node == nil_ || node->balance_ == 0) {
      return;
    } else if (node->balance_ == 1 || node->balance_ == -1) {
      if (node->parent_->left_ == node)
        --node->parent_->balance_;
      else
        ++node->parent_->balance_;
      balance_insert(node->parent_);
    } else if (node->balance_ == 2) {
      Node *rchild = node->right_;
      if (rchild->balance_ == 1) {
        rotate_left(node);
        node->balance_ = 0;
        rchild->balance_ = 0;
      } else {
        Node *rlchild = rchild->left_;
        rotate_right(rchild);
        rotate_left(node);

        rchild->balance_ = 0;
        node->balance_ = 0;

        if (rlchild->balance_ == 1)
          node->balance_ = -1;
        else if (rlchild->balance_ == -1)
          rchild->balance_ = 1;

        rlchild->balance_ = 0;
      }
    } else if (node->balance_ == -2) {
      Node *lchild = node->left_;
      if (lchild->balance_ == -1) {
        rotate_right(node);
        node->balance_ = 0;
        lchild->balance_ = 0;
      } else {
        Node *lrchild = lchild->right_;
        rotate_left(lchild);
        rotate_right(node);

        lchild->balance_ = 0;
        node->balance_ = 0;

        if (lrchild->balance_ == 1)
          lchild->balance_ = -1;
        else if (lrchild->balance_ == -1)
          node->balance_ = 1;

        lrchild->balance_ = 0;
      }
    }
  }

  Node *find_impl(Node *node, std::string key) {
    if (node == nil_) {
      nil_->parent_ = nil_;
      return nil_;
    }
    if (node->key_ < key) {
      if (node->right_ == nil_) {
        nil_->parent_ = node;
        return nil_;
      }
      return find_impl(node->right_, key);
    } else if (node->key_ > key) {
      if (node->left_ == nil_) {
        nil_->parent_ = node;
        return nil_;
      }
      return find_impl(node->left_, key);
    }

    return node;
  }

  void insert_impl(Node *node, std::string &key, uint64_t data) {
    Node *result = find_impl(node, key);
    if (result != nil_) {
      result->data_ = data;
      return;
    }

    result = result->parent_;
    Node *newNode = new Node{key, data, result, nil_, nil_};
    if (result->key_ < key) {
      result->right_ = newNode;
      ++result->balance_;
    } else {
      result->left_ = newNode;
      --result->balance_;
    }

    balance_insert(result);
  }

  void print_impl(Node *node, int depth) {
    if (node == nil_)
      return;

    print_impl(node->right_, depth + 1);
    for (int i = 0; i < depth; ++i)
      std::cout << '\t';
    std::cout << "key=" << node->key_ << ' ' << "val=" << node->data_
              << '\n';
    print_impl(node->left_, depth + 1);
  }
  void balance_remove(Node *node) {
    if (node == nil_ || node->balance_ == 1 || node->balance_ == -1)
      return;
    if (node->balance_ == 0) {
      if (node->parent_->left_ == node)
        ++node->parent_->balance_;
      else
        --node->parent_->balance_;
      balance_remove(node->parent_);
    } else if (node->balance_ == 2) {
      Node *rchild = node->right_;
      Node *rlchild = rchild->left_;
      switch (rchild->balance_) {
      case 1:
        rotate_left(node);
        node->balance_ = 0;
        rchild->balance_ = 0;
        balance_remove(rchild);
        break;
      case 0:
        rotate_left(node);
        node->balance_ = 1;
        rchild->balance_ = -1;
        return;
      case -1:
        rotate_right(rchild);
        rotate_left(node);
        node->balance_ = 0;
        rchild->balance_ = 0;
        if (rlchild->balance_ == 1)
          node->balance_ = -1;
        else if (rlchild->balance_ == -1)
          rchild->balance_ = 1;
        rlchild->balance_ = 0;
        balance_remove(rlchild);
        break;
      }
    } else if (node->balance_ == -2) {
      Node *lchild = node->left_;
      Node *lrchild = lchild->right_;
      switch (lchild->balance_) {
      case -1:
        rotate_right(node);
        node->balance_ = 0;
        lchild->balance_ = 0;
        balance_remove(lchild);
        break;
      case 0:
        rotate_right(node);
        node->balance_ = -1;
        lchild->balance_ = 1;
        return;
      case 1:
        rotate_left(lchild);
        rotate_right(node);
        node->balance_ = 0;
        lchild->balance_ = 0;
        if (lrchild->balance_ == 1)
          lchild->balance_ = -1;
        else if (lrchild->balance_ == -1)
          node->balance_ = 1;
        lrchild->balance_ = 0;
        balance_remove(lrchild);
        break;
      }
    }
  }

  void remove_impl(Node *node, std::string &key) {
    if (node->right_ == nil_ && node->left_ == nil_) {
      if (node == root_) {
        root_ = nil_;
      } else {
        if (node->parent_->left_ == node) {
          node->parent_->left_ = nil_;
          ++node->parent_->balance_;
        } else {
          node->parent_->right_ = nil_;
          --node->parent_->balance_;
        }
      }
      balance_remove(node->parent_);
      delete node;
    } else if (node->left_ == nil_ || node->right_ == nil_) {
      Node *replacingNode;
      if (node->left_ != nil_)
        replacingNode = node->left_;
      else
        replacingNode = node->right_;

      if (node->parent_->left_ == node) {
        node->parent_->left_ = replacingNode;
        ++node->parent_->balance_;
      } else {
        node->parent_->right_ = replacingNode;
        --node->parent_->balance_;
      }
      replacingNode->parent_ = node->parent_;
      if (node == root_)
        root_ = replacingNode;
      delete node;
      balance_remove(replacingNode->parent_);
    } else {
      Node *nextNode = node->right_;
      while (nextNode->left_ != nil_)
        nextNode = nextNode->left_;

      node->key_ = nextNode->key_;
      node->data_ = nextNode->data_;
      remove_impl(nextNode, nextNode->key_);
    }
  }

  void serialize_impl(Node *node, std::ostream &os) {
    if (node == nil_) {
      os << "| ";
      return;
    }
    os << node->key_ << ' ' << node->data_ << ' ';
    serialize_impl(node->left_, os);
    serialize_impl(node->right_, os);
  }

  Node *deserialize_impl(std::istream &is) {
    std::string word, strData;
    size_t data;
    is >> word;
    // if(word == "")
    //   throw std::runtime_error("Undefined format");
    if (word == "|")
      return nil_;

    is >> strData;
    // for(char c : strData)
    //   if(!(c >= '0' && c <= '9'))
    //     throw std::runtime_error("Undefined format");
    
    data = std::atoll(strData.c_str());

    Node *left = deserialize_impl(is);
    Node *right = deserialize_impl(is);

    return new Node{word, data, nil_, left, right};
  }

  void destroy(Node *node) {
    if (node == nil_)
      return;
    destroy(node->left_);
    destroy(node->right_);

    delete node;
  }

public:
  AVLTree() {
    nil_ = new Node{"", 0, nullptr, nullptr, nullptr};
    nil_->left_ = nil_;
    nil_->right_ = nil_;
    nil_->parent_ = nil_;

    root_ = nil_;
  }
  ~AVLTree() {
    destroy(root_);
    delete nil_;
  }

  std::optional<uint64_t> find(std::string key) {
    Node *result = find_impl(root_, key);
    if (result == nil_)
      return std::nullopt;
    return {result->data_};
  }
  void serialize(std::ostream &os) { 
    serialize_impl(root_, os); 
  }

  void deserialize(std::istream &is) {
    Node *temp = root_;
    root_ = nil_;

    try {
      root_ = deserialize_impl(is);
    } catch (std::exception &e) {
      destroy(root_);
      root_ = temp;
      throw;
    }
    destroy(temp);
  }

  void insert(std::string &key, uint64_t data) {
    if (root_ == nil_) {
      root_ = new Node{key, data, nil_, nil_, nil_};
      return;
    }

    insert_impl(root_, key, data);
  }

  void remove(std::string &key) {
    Node *findResult = find_impl(root_, key);
    if (findResult == nil_)
      return;

    remove_impl(findResult, key);
  }

  void print() { print_impl(root_, 0); }
};

void toLower(std::string &s) {
  for (char &c : s)
    c = std::tolower(c);
}

bool isWritable(std::string path) {
  using namespace std::filesystem;
  file_status fstat = status(path);
  return perms::none !=
         (fstat.permissions() &
          (perms::group_write | perms::owner_write | perms::others_write));
}

bool isReadable(std::string path) {
  using namespace std::filesystem;
  file_status fstat = status(path);
  return perms::none !=
         (fstat.permissions() &
          (perms::group_read | perms::owner_read | perms::others_read));
}

bool isExist(std::string path) {
  return std::filesystem::exists(path);
}

int main() {
  std::ios::sync_with_stdio(false);
  // std::cin.tie(0); std::cout.tie(0);

  AVLTree tree;
  std::string command, word;
  size_t data;
  while (std::cin >> command) {
    try {
      if (command == "+") {
        std::cin >> word >> data;
        toLower(word);
        if (tree.find(word).has_value())
          std::cout << "Exist" << '\n';
        else {
          tree.insert(word, data);
          std::cout << "OK" << '\n';
        }
      } else if (command == "-") {
        std::cin >> word;
        toLower(word);
        if (tree.find(word).has_value()) {
          tree.remove(word);
          std::cout << "OK" << '\n';
        } else
          std::cout << "NoSuchWord" << '\n';
      } else if (command == "!") {
        std::string filePath;
        std::cin >> command >> filePath;
        if (command == "Save") {
            std::ofstream os(filePath);
            tree.serialize(os);
        } else if (command == "Load") {
          if (isExist(filePath)) {
            std::ifstream is(filePath);
            tree.deserialize(is);
          } else {
            std::cout << "ERROR: " << "can't read from file" << '\n';
            continue;
          }
        }
        std::cout << "OK" << std::endl;
      } else {
        word = command;
        toLower(word);
        auto findResult = tree.find(word);
        if (findResult.has_value()) {
          std::cout << "OK: " << findResult.value() << '\n';
        } else
          std::cout << "NoSuchWord" << '\n';
      }
    } catch (std::exception &e) {
      std::cout << "ERROR: " << e.what() << '\n';
      continue;
    }
  #ifdef DEBUG
    tree.print();
  #endif // !DEBUG
  }
  return 0;
}
