#ifndef SUFFIX_TRIE_HPP_
#define SUFFIX_TRIE_HPP_

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class SuffixTrie {
  /* Realization of Ukkonen's algorithm */
  private:
    const std::string & text_;
    char sentinel;
    std::shared_ptr<size_t> endPos_ = std::make_shared<size_t>(-1); // Index of last inserted letter in trie
    size_t leafs_ = 0; // Used for detecting start of suffix
    
    struct Node {
      std::pair<size_t, std::shared_ptr<size_t>> slice; // Slice of text on arc, right border may be changed from another places, so it ptr
      std::map<char, Node*> translations;
      Node * link = nullptr; // Suffix link
      Node * parent = nullptr;
      std::optional<size_t> suffixStartPos = std::nullopt; // Index of suffix ends here (nullopt for non-leaf nodes)

      bool canGoTo(const char &) const;
      size_t getSliceSize() const;
    };

    struct ActivePoint : public std::pair<Node*, size_t> {
      ActivePoint(Node * node, size_t shift) {
        first = node;
        second = shift;
      }

      Node*& node() {
        return first;
      }
      size_t & shift() {
        return second;
      }
    };
    Node * root_ = nullptr;
    Node * lastNode_ = nullptr; // Last inserted node for splitted arc
                                
    // Split arc by given position, letter at text_[i] inserted as translation on new arc 
    // Return node has created on arc or splitted node
    Node * splitArc(ActivePoint position, size_t i);

    // Split node by given position, letter at text_[i] inserted as new translation from node 
    void splitNode(Node * node, size_t i);

    // Split cascade of suffixes by letter at text_[i] by decreasing suffixes length
    ActivePoint splitCascade(ActivePoint position, size_t i);

    // Run back for smaller suffix by suffix link or run futher and set suffix link
    // i is index of current char in text_
    // Return position of smaller suffix
    ActivePoint runBack(ActivePoint position, size_t i);

    void deleteNode(Node * node);
    void createTrie();

    void printTree(Node * node, size_t depth = 0);

    // Find end nodes by depth search
    std::vector<Node *> findEndNodes(Node * node) const;

    friend class SuffixArray;
  public:
    SuffixTrie(const std::string &text);

    std::vector<size_t> find(const std::string & pattern) const;

    /**
     * @brief Find minimal slice in lexicographical sense with given size
     *
     * @param slizeSize -- size of slice
     *
     * @return index in text the slice begin with
     */
    size_t findMinSlice(size_t sliceSize);

    ~SuffixTrie();
};


#endif // !SUFFIX_TRIE_HPP_
