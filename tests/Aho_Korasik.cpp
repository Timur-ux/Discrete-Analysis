#include <gtest/gtest.h>
#include <string>
#include "Aho_Korasik.hpp"


TEST(StringsTestSuite, Aho_Korasik) {
  std::vector<std::string> patterns{"catafalk", "taf", "cat", "dog"};
  std::string s = "catafalk catch cat and dog said -- taf";

  occurences expected{
    {"catafalk", {1, {{0, 1}}}},
    {"taf", {2, {{2, 1}, {35, 8}}}},
    {"cat", {3, {{0, 1}, {9, 2}, {15, 3}}}},
    {"dog", {1, {{23, 5}}}}
  };

  AKTrie akTrie(patterns);

  occurences given = akTrie.find(s);

  ASSERT_EQ(expected, given);
}
