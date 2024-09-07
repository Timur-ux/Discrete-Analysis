#include "SuffixArray.hpp"
#include "SuffixTrie.hpp"

void SuffixArray::createFromTrie(SuffixTrie::Node * node, size_t totalLen) {
  for (auto & [_, child] : node->translations) {
    if(!child->translations.empty())
      createFromTrie(child, totalLen + (*node->slice.second - node->slice.first));
    else {
      suffixIndexes_.push_back(child->suffixStartPos.value());
      // Add LCD Manipulation here
    }

  }
}

SuffixArray::SuffixArray(const SuffixTrie & trie) {
  suffixIndexes_.reserve(trie.text_.size());

  createFromTrie(trie.root_, 0);
  suffixIndexes_.erase(std::begin(suffixIndexes_));
}

std::vector<size_t> SuffixArray::find(const std::string & pattern) const {
  // Todo
};
