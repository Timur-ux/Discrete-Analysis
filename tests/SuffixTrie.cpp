#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "SuffixTrie.hpp"

#define all(x) std::begin(x), std::end(x)

using TestPattern = std::pair<std::string, std::vector<size_t>>;

TEST(StringsTestSuite, SuffixTrie) {
  // std::string text = "catafalk catch cat and dog said -- taf$";
  std::string text = "AABAABCAABCD$";
  TestPattern pattern1 = { "AB", {1, 4, 8}};
  std::sort(all(pattern1.second));

  SuffixTrie trie(text);
  std::vector<size_t> indexes = trie.find(pattern1.first);
  std::sort(all(indexes));

  EXPECT_EQ(indexes.size(), pattern1.second.size());
  
  for(size_t i = 0; i < indexes.size(); ++i)
    EXPECT_EQ(indexes[i], pattern1.second[i]);
}
