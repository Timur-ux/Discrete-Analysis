#ifndef AHO_KORASIK_HPP
#define AHO_KORASIK_HPP

#include <map>
#include <optional>
#include <queue>
#include <string>
#include <vector>

struct occurence {
  size_t count;
  std::vector<size_t> positions;

  bool operator<=>(const occurence & other) const = default;
};

using occurences = std::map<std::string, occurence>; 

class AKTrie {
  public:
    AKTrie(std::vector<std::string> & patterns);
    occurences find(std::string& s) {
      return {}; // TODO
    }
  private:
    struct Node {
      char letter;
      std::map<char, size_t> translations;
      size_t lp; // Longest prefix
      std::optional<size_t> el; // Exit link
      bool isTerm;

      Node(char l) : letter(l), lp(0), isTerm(false) {}
    };

    std::vector<Node> nodes;
    size_t rootId;

    size_t createNode(char letter = 0);
    void insertPattern(std::string &p);
    void setLinks();

    /* 
     * linking all childs of nodesToLink.front()
     * and recursive calls itself. Aka breadth search */
    void setLinks_impl(std::queue<size_t>& nodesToLink); 
};

#endif // !AHO_KORASIK_HPP
