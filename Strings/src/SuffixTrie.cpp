#include "SuffixTrie.hpp"

SuffixTrie::SuffixTrie(const std::string &text) : text_(text) {
  root_ = new Node{.slice = {0, std::make_shared<size_t>(0)}};
  createTrie();
}

void SuffixTrie::deleteNode(Node *node) {
  if (node->translations.empty()) {
    delete node;
    return;
  }

  for (auto &[_, child] : node->translations)
    deleteNode(child);

  delete node;
}

SuffixTrie::~SuffixTrie() { deleteNode(root_); }

bool SuffixTrie::Node::canGoTo(const char &c) const {
  return translations.find(c) != translations.end();
}

std::pair<SuffixTrie::Node *, SuffixTrie::Node *>
SuffixTrie::splitArc(std::pair<Node *, size_t> position, size_t i) {
  auto &[node, shift] = position;

  if (node == root_) {
    Node *newNode = (root_->translations[text_[i]] = new Node{
                         .slice = {leafs_, endPos_}, .startPos = leafs_});

    ++leafs_;
    lastNode_ = nullptr;

    return {root_, newNode};
  }

  Node *oldNode =
      (node->translations[text_[node->slice.first + shift]] =
           new Node{.slice = {node->slice.first + shift, node->slice.second},
                    .startPos = node->startPos});
  Node *newNode = (node->translations[text_[i]] =
                       new Node{.slice = {i, endPos_}, .startPos = leafs_});

  node->slice.second = std::make_shared<size_t>(node->slice.first + shift);
  node->startPos = std::nullopt;

  ++leafs_;
  if (lastNode_ != nullptr)
    lastNode_->link = node;
  lastNode_ = node;

  return {oldNode, newNode};
}

void SuffixTrie::createTrie() {
  std::pair<Node *, size_t> activePoint = {root_, 0};
  for (size_t i = 0; i < text_.size(); ++i) {
    ++(*endPos_);
    auto &[node, shift] = activePoint;

    if (shift < *node->slice.second - node->slice.first) {
      if (text_[node->slice.first + shift] == text_[i]) {
        ++shift;
        continue;
      } else {
        auto [oldNode, newNode] = splitArc(activePoint, i);
        
      }
    }
  }
}
