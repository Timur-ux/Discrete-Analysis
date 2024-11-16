#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <map>
#include <string>
#include <vector>

#define in(container, item) (container.find(item) != container.end())

class Trie {
public:
  Trie();
  Trie(std::vector<std::string> &);

  void insert(std::string &);
  void remove(std::string &);

  bool find(std::string &);

private:
  char terminator = '$';
  struct Node {
    std::map<char, Node *> translations;
  };

  Node *root;
  // void impl_remove(std::string &, size_t curIndex, Node *currentNode);
};

inline Trie::Trie() {
  root = new Node{std::map<char, Node *>{{terminator, nullptr}}};
}

inline Trie::Trie(std::vector<std::string> &data) : Trie() {
  for (std::string &s : data) {
    insert(s);
  }
}

inline void Trie::insert(std::string &s) {
  if (find(s)) {
    return;
  }

  Node *currentNode = root;
  for (char &c : s) {
    if (!in(currentNode->translations, c)) {
      currentNode->translations[c] = new Node;
    }
    currentNode = currentNode->translations[c];
  }
  currentNode->translations[terminator] = nullptr;
}

inline bool Trie::find(std::string &s) {
  Node *currentNode = root;
  for (char &c : s) {
    if (!in(currentNode->translations, c)) {
      return false;
    }
    currentNode = currentNode->translations[c];
  }

  return in(currentNode->translations, terminator);
}

inline void Trie::remove(std::string &s) {
  if (!find(s)) {
    return;
  }

  size_t curIndex = 0;
  Node *currentNode = root;

  std::vector<Node *> path;
  path.reserve(s.size());

  while (curIndex < s.size()) {
    path.emplace_back(currentNode);
    currentNode = currentNode->translations[s[curIndex]];
    ++curIndex;
  }

  currentNode->translations.erase(terminator);
  
  --curIndex;
  currentNode = path.back();
  path.pop_back();

  while (curIndex >= 0 && curIndex < s.size() 
         && currentNode->translations[s[curIndex]]->translations.size() == 0) {
    currentNode->translations.erase(s[curIndex]);
    currentNode = path.back();
    path.pop_back();
    --curIndex;
  }
}

#undef in

#endif // TRIE_HPP_
