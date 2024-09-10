#ifndef SUFFIX_ARRAY_HPP_
#define SUFFIX_ARRAY_HPP_

#include "SuffixTrie.hpp"
#include <vector>


class SuffixArray {
  private:
    const std::string &text_;
    std::vector<size_t> suffixIndexes_;
    std::vector<size_t> LCP_;
    /**
     * @brief recurcive method for constructing suffix array from trie
     * Assumed that trie is $-terminated
     * @param node -- current node
     * @param commonSuffixLen -- common suffix len between this and last inserted nodes
     * @param totalLen -- length from root to current node
     */
    void createFromTrie(SuffixTrie::Node * node, size_t &commonSuffixLen, size_t totalLen = 0);

    /**
     * @brief filling LCP_ for non neiborhood positions
     *
     * @param L -- left border
     * @param R -- right border
     * @param newLCP -- full LCP vector will be constructed at fillLCP(0, n) end
     *
     * @return longest common prefix for suffixes on L and R positions
     */
    size_t fillLCP(size_t L, size_t R, std::vector<size_t> & newLCP);

    /**
     * @brief return LCP value for L and R pair
     *
     * @param L -- lower index in suffixIndexes_
     * @param R -- higher index in suffixIndexes_
     *
     * @return Longest common prefix for values L and R
     */
    size_t getLCPAt(size_t L, size_t R) const;

    void printLCPStructure(size_t L, size_t R, const std::string & text) const;
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
