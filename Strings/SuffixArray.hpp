#ifndef SUFFIX_ARRAY_HPP_
#define SUFFIX_ARRAY_HPP_

#include "SuffixTrie.hpp"
#include <vector>


class SuffixArray {
  private:
    std::vector<size_t> suffixIndexes_;
    std::map<std::pair<size_t, size_t>, size_t> LCP_; // Longest common prefix;

    /**
     * @brief recurcive method for constructing suffix array from trie
     *
     * @param node -- current node
     * @param totalLen -- length from root to current node
     */
    void createFromTrie(SuffixTrie::Node * node, size_t totalLen);
  public:
    /**
     * @brief Create suffix array from suffix trie
     *
     * @param trie -- Suffix trie
     */ 
    SuffixArray(const SuffixTrie & trie);

    /**
     * @brief Find all pattern occurences in text
     *
     * @param pattern
     *
     * @return vector with indexes of all pattern's occurences
     */
    std::vector<size_t> find(const std::string & pattern) const;
};

#endif // !SUFFIX_ARRAY_HPP_
