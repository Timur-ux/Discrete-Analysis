#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "SuffixTrie.hpp"

SuffixTrie::SuffixTrie(const std::string &text)
    : text_(text), sentinel(text[text.size() - 1]) {
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

size_t SuffixTrie::Node::getSliceSize() const {
  return *slice.second - slice.first;
}

SuffixTrie::Node *SuffixTrie::splitArc(SuffixTrie::ActivePoint position,
                                       size_t i) {
  Node *newNode = new Node{
      .slice = {position.node()->slice.first,
                std::make_shared<size_t>(position.node()->slice.first +
                                         position.shift())},
      .translations = {{text_[position.node()->slice.first + position.shift()],
                        position.node()}},
      .parent = position.node()->parent,
      .suffixStartPos = std::nullopt};
  position.node()->parent = newNode;
  position.node()->slice.first = position.node()->slice.first + position.shift();
  newNode->parent->translations[text_[newNode->slice.first]] = newNode;

  splitNode(newNode, i);

  return newNode;
}

void SuffixTrie::splitNode(SuffixTrie::Node *node, size_t i) {
  node->translations[text_[i]] = new Node{
      .slice = {i, endPos_}, .parent = node, .suffixStartPos = leafs_++};
}

SuffixTrie::ActivePoint SuffixTrie::runBack(SuffixTrie::ActivePoint position,
                                            size_t i) {
  if (position.node() == root_) {
    std::cerr << "ERROR: the try to run back from root_ has detected"
              << std::endl;
    exit(1);
  }
  if(position.node()->link != nullptr) {
    return {position.node()->link, position.node()->link->getSliceSize()};
  }

  size_t totalShift = 0;
  while (position.node() != root_ && position.node()->link == nullptr) {
    totalShift += position.node()->getSliceSize();
    position.node() = position.node()->parent;
  }
  if (position.node() == root_ && totalShift == 0) {
    std::cerr << "ERROR: run back to root_ but total shift is 0" << std::endl;
    exit(1);
  }
  if (position.node() == root_)
    --totalShift;
  else
    position.node() = position.node()->link;

  if (totalShift > 0) {
    position.node() = position.node()->translations[text_[i - totalShift]];
    while (totalShift > position.node()->getSliceSize()) {
      totalShift -= position.node()->getSliceSize();
      position.node() = position.node()->translations[text_[i - totalShift]];
    }
  }

  return {position.node(), totalShift};
}

SuffixTrie::ActivePoint
SuffixTrie::splitCascade(SuffixTrie::ActivePoint position, size_t i) {
  if (position.shift() > position.node()->getSliceSize()) {
    std::cerr << "ERROR: Shift bigger than slice size"
              << std::endl; // For testing purposes
    exit(1);
  }
  Node *lastSplitted = nullptr;
  while (
      position.node() != root_ &&
      ((position.shift() == position.node()->getSliceSize() &&
        !position.node()->canGoTo(text_[i])) ||
       (position.shift() < position.node()->getSliceSize() &&
        text_[position.node()->slice.first + position.shift()] != text_[i]))) {

    if (position.shift() ==
        position.node()->getSliceSize()) { // We at node, not at arc. So we need
                                           // add new translation to node
      splitNode(position.node(), i);
      if (lastSplitted != nullptr)
        lastSplitted->link = position.node();
      lastSplitted = position.node();
    } else { // We at arc, so we split arc.
      position.first = splitArc(position, i);
      position.second = position.node()->getSliceSize();

      if (lastSplitted != nullptr)
        lastSplitted->link = position.node();
      lastSplitted = position.node();
    }
    position = runBack(position, i);
  }

  if (position.node() == root_ && !position.node()->canGoTo(text_[i]))
    splitNode(position.node(), i);
  else if (position.shift() == position.node()->getSliceSize() &&
           position.node()->canGoTo(text_[i])) {
    position.first = position.node()->translations[text_[i]];
    position.second = 1;
  } else if (position.shift() < position.node()->getSliceSize() &&
        text_[position.node()->slice.first + position.shift()] != text_[i]) {
    ++position.shift();
  } else {
    std::cerr << "ERROR: undefined case in splitCascade" << std::endl;
    exit(1);
  }

  return position;
}

void SuffixTrie::printTree(SuffixTrie::Node *node, size_t depth) {
  std::cout << text_.substr(node->slice.first,
                            *node->slice.second - node->slice.first);
  if (node->link == nullptr)
    std::cout << "; link: none" << std::endl;
  else
    std::cout << "; link: "
              << text_.substr(node->link->slice.first,
                              *node->link->slice.second -
                                  node->link->slice.first)
              << std::endl;

  for (auto [letter, child] : node->translations) {
    if (letter != text_[child->slice.first])
      std::cout << "ERROR: translation letter don't correspond real slice"
                << std::endl;
    for (size_t i = 0; i < depth; ++i)
      std::cout << "  ";
    std::cout << letter << " : ";
    printTree(child, depth + 1);
  }
}

void SuffixTrie::createTrie() {
  ActivePoint ap = {root_, 0};
  for (size_t i = 0; i < text_.size(); ++i) {
    ++(*endPos_);
    if (ap.shift() < ap.node()->getSliceSize()) {
      if (text_[ap.node()->slice.first + ap.shift()] == text_[i])
        ++ap.shift();
      else
        ap = splitCascade(ap, i);
    } else {
      if (ap.node()->canGoTo(text_[i]))
        ap = {ap.node()->translations[text_[i]], 1};
      else
        ap = splitCascade(ap, i);
    }
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
  for (auto node : endNodes)
    result.push_back(node->suffixStartPos.value());

  return result;
}

std::vector<SuffixTrie::Node *>
SuffixTrie::findEndNodes(SuffixTrie::Node *node) const {
  if (node->suffixStartPos.has_value())
    return {node};

  std::vector<Node *> result;
  for (auto &[_, child] : node->translations) {
    std::vector<Node *> childEndNodes = findEndNodes(child);
    for (auto endNode : childEndNodes)
      result.push_back(endNode);
  }

  return result;
}
size_t SuffixTrie::findMinSlice(size_t sliceSize) {
  Node *currentNode = root_;
  while (sliceSize > *currentNode->slice.second - currentNode->slice.first) {
    sliceSize -= *currentNode->slice.second - currentNode->slice.first;
    auto translation = std::begin(currentNode->translations);
    if (translation->first == sentinel)
      ++translation;
    currentNode = translation->second;
  }

  while (currentNode->translations.size() > 0) {
    auto translation = std::begin(currentNode->translations);
    if (translation->first == sentinel && currentNode->translations.size() > 1)
      ++translation;
    currentNode = translation->second;
  }

  return currentNode->suffixStartPos.value();
}
