#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "SuffixArray.hpp"

#define all(x) std::begin(x), std::end(x)

using TestPattern = std::pair<std::string, std::vector<size_t>>;

TEST(StringsTestSuite, SuffixArray) {
  // std::string text = "catafalk catch cat and dog said -- taf$";
  std::string text = "AABAABCAABCD$";
  TestPattern pattern1 = { "AB", {1, 4, 8}};
  SuffixTrie trie(text);
  SuffixArray suffixArray(trie);

  std::vector<size_t> indexes = suffixArray.find(pattern1.first);
  std::sort(all(indexes));
  std::sort(all(pattern1.second));

  std::cout << "given: ";
  for(const auto & id : indexes)
    std::cout << id << ' ';
  std::cout << std::endl << "expected: ";
  for(const auto & id : pattern1.second)
    std::cout << id << ' ';
  std::cout << std::endl;

  EXPECT_EQ(indexes.size(), pattern1.second.size());
  for(size_t i = 0; i < indexes.size(); ++i)
    EXPECT_EQ(indexes[i], pattern1.second[i]);
}

