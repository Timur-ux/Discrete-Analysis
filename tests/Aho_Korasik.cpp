#include <gtest/gtest.h>
#include <string>
#include "Aho_Korasik.hpp"


TEST(StringsTestSuite, Aho_Korasik) {
  std::vector<std::string> patterns{"catafalk", "taf", "cat", "dog"};
  std::string s = "catafalk catch cat and dog said -- taf";

  occurences expected{
    {"catafalk", {1, {0}}},
    {"taf", {2, {2, 35}}},
    {"cat", {3, {0, 9, 15}}},
    {"dog", {1, {23}}}
  };

  AKTrie akTrie(patterns);

  occurences given = akTrie.find(s);

  ASSERT_EQ(expected, given);
}
