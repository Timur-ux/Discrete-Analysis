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
    Node *newNode =
        (root_->translations[text_[i]] = new Node{.slice = {leafs_, endPos_},
                                                  .parent = root_,
                                                  .suffixStartPos = leafs_});

    ++leafs_;
    return {root_, newNode};
  }

  Node *oldNode =
      (node->translations[text_[node->slice.first + shift]] =
           new Node{.slice = {node->slice.first + shift, node->slice.second},
                    .parent = node,
                    .suffixStartPos = node->suffixStartPos});
  Node *newNode =
      (node->translations[text_[i]] = new Node{
           .slice = {i, endPos_}, .parent = node, .suffixStartPos = leafs_});

  node->slice.second = std::make_shared<size_t>(node->slice.first + shift);
  node->suffixStartPos = std::nullopt;
  ++leafs_;

  return {oldNode, newNode};
}

std::pair<SuffixTrie::Node *, size_t>
SuffixTrie::runBack(std::pair<Node *, size_t> position, size_t i) {
  auto &[node, totalShift] = position;
  while (node->link == nullptr && node != root_) {
    node = node->parent;
    totalShift += *node->slice.second - node->slice.first;
  }

  if (node->link != nullptr)
    node = node->link;

  node = node->translations[text_[i - totalShift]];
  while(totalShift > *node->slice.second - node->slice.first) {
    totalShift -= *node->slice.second - node->slice.first;
    node = node->translations[text_[i - totalShift]];
  }

  size_t shift = 0;
  while(totalShift > shift && text_[i - totalShift] == text_[node->slice.first + shift])
    shift++;

  return {node, shift};
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
        // TODO: run back for smaller suffixes
      }
    }
  }
}
