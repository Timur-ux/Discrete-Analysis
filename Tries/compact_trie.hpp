#ifndef COMPACT_TRIE_HPP_
#define COMPACT_TRIE_HPP_
#include <chrono>
#include <map>
#include <string>
#include <vector>

class CompactTrie {
public:
  CompactTrie();
  CompactTrie(std::vector<std::string> &);
  void insert(std::string );
  void remove(std::string);
  bool find(std::string);

private:
  struct Node {
    std::string substr_ = "";
    std::map<char, Node *> translations_;
  };

  struct ActivePoint {
    Node *node;
    size_t offset;
  };

  Node *root_;
};

inline CompactTrie::CompactTrie() { 
  root_ = new Node;
  insert("");
}

inline CompactTrie::CompactTrie(std::vector<std::string> &data)
    : CompactTrie() {
  for (auto &str : data) {
    insert(str);
  }
}

inline void CompactTrie::insert(std::string str) {
  str += '$';
  ActivePoint activePoint{root_, 0};
  for (size_t i = 0; i < str.size(); ++i) {
    if (activePoint.offset < activePoint.node->substr_.size()) {
      if (activePoint.node->substr_[activePoint.offset] == str[i]) {
        ++activePoint.offset;
        continue;
      } else {
        std::string subleft = activePoint.node->substr_.substr(0, activePoint.offset);
        std::string subright =
            activePoint.node->substr_.substr(activePoint.offset, std::string::npos);

        Node *splittedPart =
            new Node{subright, std::move(activePoint.node->translations_)};
        Node *newNode = new Node{str.substr(i, std::string::npos), {}};

        activePoint.node->substr_ = subleft;
        activePoint.node->translations_ = {{subright[0], splittedPart},
                                           {str[i], newNode}};
        break;
      }
    } else {
      if (activePoint.node->translations_.find(str[i]) !=
          activePoint.node->translations_.end()) {
        activePoint.node = activePoint.node->translations_[str[i]];
        activePoint.offset = 1;
      } else {
        Node *newNode = new Node{str.substr(i, std::string::npos), {}};
        activePoint.node->translations_[str[i]] = newNode;
        break;
      }
    }
  }
}

inline bool CompactTrie::find(std::string str) {
  str += '$';
  ActivePoint activePoint = {root_, 0};
  for (char &c : str) {
    if(activePoint.offset < activePoint.node->substr_.size()) {
      if(activePoint.node->substr_[activePoint.offset] != c) {
        return false;
      }
      ++activePoint.offset;
    } else {
      if(activePoint.node->translations_.find(c) == activePoint.node->translations_.end()) {
        return false;
      }
      activePoint.node = activePoint.node->translations_[c];
      activePoint.offset = 1;
    }
  }

  return true;
}

inline void CompactTrie::remove(std::string str) {
  if(str == "" || !find(str))
    return;

  str += '$';

  Node * currentNode = root_, * prevNode = root_;
  for(int i = 0; i < str.size(); i += currentNode->substr_.size()) {
    prevNode = currentNode;
    currentNode = currentNode->translations_[str[i]];
  }

  prevNode->translations_.erase(currentNode->substr_[0]);
  delete currentNode;

  if(prevNode == root_ || prevNode->translations_.size() > 1) 
    return;

  Node * lastNode = prevNode->translations_.begin()->second;
  prevNode->substr_ += lastNode->substr_;
  prevNode->translations_ = std::move(lastNode->translations_);
  delete lastNode;

  return;
}

#endif // !COMPACT_TRIE_HPP_
