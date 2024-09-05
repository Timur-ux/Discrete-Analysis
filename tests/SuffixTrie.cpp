#include <gtest/gtest.h>
#include <string>
#include "SuffixTrie.hpp"

using TestPattern = std::pair<std::string, std::vector<size_t>>;

TEST(StringsTestSuite, SuffixTrie) {
  // std::string text = "catafalk catch cat and dog said -- taf$";
  std::string text = "AABAABCAABCD$";
  // TestPattern pattern1 = { "cat", {0, 9, 15}};

  SuffixTrie trie(text);

  
  ASSERT_TRUE(false);
}
