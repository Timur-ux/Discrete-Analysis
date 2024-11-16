#include "Aho_Korasik.hpp"
#include <cstddef>
#include <queue>

#define in(map, item) (map.find(item) != map.end())

AKTrie::AKTrie(std::vector<std::string> &patterns) : patterns_(patterns) {
  rootId = createNode();
  for (size_t i = 0; i < patterns_.size(); ++i) {
    insertPattern(patterns_[i], i);
  }

  setLinks();
}

void AKTrie::insertPattern(std::string &p, size_t patternIndex) {
  size_t curNodeId = rootId;
  for (char &c : p) {
    if (!in(nodes[curNodeId].translations, c)) {
      nodes[curNodeId].translations[c] = createNode(c);
    }
    curNodeId = nodes[curNodeId].translations[c];
  }

  nodes[curNodeId].isTerm = true;
  nodes[curNodeId].patternIndex_ = patternIndex;
}

void AKTrie::setLinks() {
  nodes[rootId].lp = rootId;
  std::queue<size_t> nodesToLink;

  for (auto &[letter, id] : nodes[rootId].translations) {
    nodes[id].lp = rootId;
    nodesToLink.push(id);
  }

  setLinks_impl(nodesToLink);
}

void AKTrie::setLinks_impl(std::queue<size_t> &nodesToLink) {
  if (nodesToLink.empty())
    return;

  size_t nodeId = nodesToLink.front();
  nodesToLink.pop();

  for (auto &[letter, id] : nodes[nodeId].translations) {
    size_t linkedNode = nodes[nodeId].lp;
    while (linkedNode != rootId &&
           !in(nodes[linkedNode].translations, letter)) {
      linkedNode = nodes[linkedNode].lp;
    }

    if (in(nodes[linkedNode].translations, letter)) {
      nodes[id].lp = nodes[linkedNode].translations[letter];

      if (nodes[nodes[id].lp].isTerm)
        nodes[id].el = nodes[id].lp;
      else if (nodes[nodes[id].lp].el.has_value()) {
        size_t elFinal = nodes[nodes[id].lp].el.value();
        while (!nodes[elFinal].isTerm)
          elFinal = nodes[elFinal].el.value();

        nodes[id].el = elFinal;
      }
    } else {
      nodes[id].lp = rootId;
    }

    nodesToLink.push(id);
  }

  setLinks_impl(nodesToLink);
}

size_t AKTrie::createNode(char letter) {
  nodes.push_back(Node(letter));
  return nodes.size() - 1;
}

void insertOccurence(occurences &storage, std::string &pattern, size_t position,
                     size_t wordCount) {
  if (in(storage, pattern)) {
    ++storage[pattern].count;
    storage[pattern].matchings.push_back({position, wordCount});
  } else {
    storage[pattern] = {1, {{position, wordCount}}};
  }
}

void AKTrie::processNodeIfTerminatorOrExitLink(occurences &storage, Node &node,
                                               size_t currentPosition,
                                               size_t &currentWordCount) {
  std::string pattern = "";
  if (node.isTerm) {
    pattern = patterns_[node.patternIndex_.value()];
  } else if (node.el.has_value()) {
    pattern =
        patterns_[nodes[node.el.value()].patternIndex_.value()];
  }
  if (pattern != "") {
    insertOccurence(storage, pattern, currentPosition + 1 - pattern.size(),
                    currentWordCount);
  }
}

occurences AKTrie::find(std::string &s) {
  occurences result{};
  size_t wordCount = 1;

  Node *currentNode = &nodes[rootId];
  for (size_t i = 0; i < s.size(); ++i) {
    if(s[i] == ' ')
      ++wordCount;
    while (currentNode != &nodes[rootId] &&
           !in(currentNode->translations, s[i]))
      currentNode = &nodes[currentNode->lp];

    if (in(currentNode->translations, s[i])) {
      currentNode = &nodes[currentNode->translations[s[i]]];
      processNodeIfTerminatorOrExitLink(result, *currentNode, i, wordCount);
    }
  }

  return result;
}
