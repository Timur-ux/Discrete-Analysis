#ifndef AHO_KORASIK_HPP
#define AHO_KORASIK_HPP

#include <map>
#include <optional>
#include <queue>
#include <string>
#include <vector>

struct Matching {
  size_t position;
  size_t wordCount;
  bool operator<=>(const Matching&) const = default;
};

struct occurence {
  size_t count;
  std::vector<Matching> matchings;
  bool operator<=>(const occurence &) const = default;
};

using occurences = std::map<std::string, occurence>; 

class AKTrie {
  public:
    AKTrie(std::vector<std::string> & patterns);
    occurences find(std::string& s);
  private:
    std::vector<std::string> patterns_;
    struct Node {
      char letter;
      std::map<char, size_t> translations;
      size_t lp; // Longest prefix
      std::optional<size_t> el; // Exit link
                                
      bool isTerm;
      std::optional<size_t> patternIndex_;

      Node(char l) : letter(l), lp(0), isTerm(false) {}
    };

    std::vector<Node> nodes;
    size_t rootId;

    size_t createNode(char letter = 0);
    void insertPattern(std::string &p, size_t patternIndex);
    void setLinks();

    /* 
     * linking all childs of nodesToLink.front()
     * and recursive calls itself. Aka breadth search 
     */
    void setLinks_impl(std::queue<size_t>& nodesToLink); 

    void processNodeIfTerminatorOrExitLink(occurences & storage, Node & node, size_t currentPosition, size_t &currentWordCount);
};

#endif // !AHO_KORASIK_HPP
