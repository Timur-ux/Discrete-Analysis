#include <gtest/gtest.h>
#include <string>
#include "SuffixTrie.hpp"

using TestPattern = std::pair<std::string, std::vector<size_t>>;

TEST(StringsTestSuite, Aho_Korasik) {
  std::string text = "catafalk catch cat and dog said -- taf";
  TestPattern pattern1 = { "cat", {0, 9, 15}};


  




  ASSERT_EQ(expected, given);
}
