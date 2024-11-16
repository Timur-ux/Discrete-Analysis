#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "GeneralizedSuffixTrie.hpp"

GeneralizedSuffixTrie::GeneralizedSuffixTrie(const std::string &text,
                                             std::vector<char> sentinels)
    : text_(text), sentinels_(sentinels) {
  root_ = new Node{.slice = {0, std::make_shared<size_t>(0)}};
  createTrie();
}

void GeneralizedSuffixTrie::deleteNode(Node *node) {
  if (node->translations.empty()) {
    delete node;
    return;
  }

  for (auto &[_, child] : node->translations)
    deleteNode(child);

  delete node;
}

GeneralizedSuffixTrie::~GeneralizedSuffixTrie() { deleteNode(root_); }

bool GeneralizedSuffixTrie::Node::canGoTo(const char &c) const {
  return translations.find(c) != translations.end();
}

size_t GeneralizedSuffixTrie::Node::getSliceSize() const {
  return *slice.second - slice.first;
}

GeneralizedSuffixTrie::Node *
GeneralizedSuffixTrie::splitArc(GeneralizedSuffixTrie::ActivePoint position,
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
  position.node()->slice.first =
      position.node()->slice.first + position.shift();
  newNode->parent->translations[text_[newNode->slice.first]] = newNode;

  splitNode(newNode, i);

  return newNode;
}

void GeneralizedSuffixTrie::splitNode(GeneralizedSuffixTrie::Node *node,
                                      size_t i) {
  node->translations[text_[i]] = new Node{.slice = {i, endPos_},
                                          .parent = node,
                                          .suffixStartPos = leafs_++,
                                          .stringIndex = currentSentinel_};
}

GeneralizedSuffixTrie::ActivePoint
GeneralizedSuffixTrie::runBack(GeneralizedSuffixTrie::ActivePoint position,
                               size_t i) {
  if (position.node() == root_) {
    std::cerr << "ERROR: the try to run back from root_ has detected"
              << std::endl;
    exit(1);
  }
  if (position.node()->link != nullptr) {
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

GeneralizedSuffixTrie::ActivePoint
GeneralizedSuffixTrie::splitCascade(GeneralizedSuffixTrie::ActivePoint position,
                                    size_t i) {
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
             text_[position.node()->slice.first + position.shift()] !=
                 text_[i]) {
    ++position.shift();
  } else {
    std::cerr << "ERROR: undefined case in splitCascade" << std::endl;
    exit(1);
  }

  return position;
}

void GeneralizedSuffixTrie::printTree(GeneralizedSuffixTrie::Node *node,
                                      size_t depth) {
  if(depth > 0) std::cout << '|';
  for(size_t _ = 0; _ < depth; ++_) std::cout << '_';
  std::cout << ": L = " << node->slice.first << "; R = " << *node->slice.second << std::endl;

  for(auto & [_, child] : node->translations) {
    printTree(child, depth + 1);
  }
}

void GeneralizedSuffixTrie::createTrie() {
  ActivePoint ap = {root_, 0};
  currentSentinel_ = 0;
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
    if (text_[i] == sentinels_[currentSentinel_]) {
      ++currentSentinel_;
      endPos_ = std::make_shared<size_t>(*endPos_);
    }
  }
}

std::vector<size_t>
GeneralizedSuffixTrie::find(const std::string &pattern) const {
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

std::vector<GeneralizedSuffixTrie::Node *>
GeneralizedSuffixTrie::findEndNodes(GeneralizedSuffixTrie::Node *node) const {
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
