#include <iostream>
#include <vector>

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

  Node *oldNode = nullptr, *newNode = nullptr;
  if (!node->canGoTo(text_[node->slice.first + shift]))
    oldNode = (node->translations[text_[node->slice.first + shift]] = new Node{
                   .slice = {node->slice.first + shift, node->slice.second},
                   .parent = node,
                   .suffixStartPos = node->suffixStartPos});

  if (!node->canGoTo(text_[i]))
    newNode =
        (node->translations[text_[i]] = new Node{
             .slice = {i, endPos_}, .parent = node, .suffixStartPos = leafs_});

  node->slice.second = std::make_shared<size_t>(node->slice.first + shift);
  node->suffixStartPos = std::nullopt;
  ++leafs_;

  return {oldNode, newNode};
}

std::pair<SuffixTrie::Node *, size_t>
SuffixTrie::runBack(std::pair<Node *, size_t> position, size_t i) {
  auto &[node, _] = position;
  size_t totalShift = 0;
  while (node->link == nullptr && node != root_) {
    totalShift += *node->slice.second - node->slice.first;
    node = node->parent;
  }

  if (node->link != nullptr)
    node = node->link->translations[text_[i - totalShift]];
  else if (node == root_)
    --totalShift; // As we roll back for all suffix we should go forward by
                  // suffix without 1 letter

  while (totalShift > *node->slice.second - node->slice.first) {
    totalShift -= *node->slice.second - node->slice.first;
    node = node->translations[text_[i - totalShift]];
  }

  size_t shift = 0;
  while (totalShift > shift &&
         text_[i - totalShift + shift] == text_[node->slice.first + shift])
    shift++;

  return {node, shift};
}

std::pair<SuffixTrie::Node *, size_t>
SuffixTrie::splitCascade(std::pair<Node *, size_t> position, size_t i) {
  Node *lastSplitted = nullptr;
  while (position.first != root_) {
    auto [node, shift] = position;
    splitArc(position, i);

    if (lastSplitted != nullptr)
      lastSplitted->link = node;
    lastSplitted = node;

    position.second =
        *position.first->slice.second -
        position.first->slice.first; // Point at last letter on an arc such as
                                     // we split the arc by this letter
    position = runBack(position, i);
  }
  splitArc({root_, 0}, i);
  if (lastSplitted != nullptr)
    lastSplitted->link = root_;

  return {root_, 0};
}

void SuffixTrie::printTree(SuffixTrie::Node *node, size_t depth) {
  std::cout << "Slice: {" << node->slice.first << " ; " << *node->slice.second
            << "}";
  if (node->link == nullptr)
    std::cout << "; link: none" << std::endl;
  else
    std::cout << "; link: {" << node->link->slice.first << " ; "
              << *node->link->slice.second << "}" << std::endl;

  for (auto [letter, child] : node->translations) {
    for (size_t i = 0; i < depth; ++i)
      std::cout << "  ";
    std::cout << letter << " : ";
    printTree(child, depth + 1);
  }
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
        activePoint = splitCascade(activePoint, i);
      }
    } else {
      if (node->canGoTo(text_[i]))
        activePoint = {node->translations[text_[i]], 1};
      else
        activePoint = splitCascade(activePoint, i);
    }
    std::cout << "-------------: " << i << " :---------------" << std::endl;
    printTree(root_);
    std::cout << "-------------:--------------------" << std::endl;
  }
}

std::vector<size_t> SuffixTrie::find(const std::string &pattern) const {
  std::pair<Node *, size_t> activePoint{root_, 0};
  for (auto &c : pattern) {
    auto &[node, shift] = activePoint;
    if (shift < *node->slice.second - node->slice.first) {
      if (text_[node->slice.first + shift] == c)
        ++shift;
      else
        return {};
    } else {
      if (node->canGoTo(c)) {
        node = node->translations[c];
        shift = 1;
      } else
        return {};
    }
  }

  std::vector<size_t> result;
  std::vector<Node *> endNodes = findEndNodes(activePoint.first);
  for(auto node : endNodes)
    result.push_back(node->suffixStartPos.value());

  return result;
}

std::vector<SuffixTrie::Node *> SuffixTrie::findEndNodes(SuffixTrie::Node * node) const {
  if(node->suffixStartPos.has_value())
    return {node};

  std::vector<Node *> result;
  for(auto & [_, child] : node->translations) {
    std::vector<Node *> childEndNodes = findEndNodes(child);
    for(auto endNode : childEndNodes)
      result.push_back(endNode);
  }

  return result;
}
