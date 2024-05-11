#include "Aho_Korasik.hpp"
#include <queue>

#define in(map, item) (map.find(item) != map.end())

AKTrie::AKTrie(std::vector<std::string> & patterns) {
  rootId = createNode();
  for(std::string & p : patterns) {
    insertPattern(p);
  }

  setLinks();
}

void AKTrie::insertPattern(std::string & p) {
  size_t curNodeId = rootId;
  for(char & c : p) {
    if(!in(nodes[curNodeId].translations, c)) {
      nodes[curNodeId].translations[c] = createNode(c);
    }
    curNodeId = nodes[curNodeId].translations[c];
  }

  nodes[curNodeId].isTerm = true;
}

void AKTrie::setLinks() {
  nodes[rootId].lp = rootId;
  std::queue<size_t> nodesToLink;

  for(auto &[letter, id] : nodes[rootId].translations) {
    nodes[id].lp = rootId;
    nodesToLink.push(id);
  }

  setLinks_impl(nodesToLink);
}

void AKTrie::setLinks_impl(std::queue<size_t>& nodesToLink) {
  if(nodesToLink.empty())
    return;

  size_t nodeId = nodesToLink.front();
  nodesToLink.pop();

  for(auto &[letter, id] : nodes[nodeId].translations) {
    size_t linkedNode = nodes[nodeId].lp;
    while(linkedNode != rootId && !in(nodes[linkedNode].translations, letter)) {
      linkedNode = nodes[linkedNode].lp;
    }

    if(in(nodes[linkedNode].translations, letter)) {
      nodes[id].lp = nodes[linkedNode].translations[letter];

      if(nodes[nodes[id].lp].isTerm)
        nodes[id].el = nodes[id].lp;
      else if(nodes[nodes[id].lp].el.has_value()) {
        size_t elFinal = nodes[nodes[id].lp].el.value();
        while(!nodes[elFinal].isTerm)
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

bool operator==(const occurence &lhs, const occurence & rhs) {
  return lhs.count == rhs.count && lhs.positions == rhs.positions;
}
