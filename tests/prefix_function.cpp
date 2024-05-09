#include "prefix_function.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(StringsTestSuite, prefix_functionBuilding) {
  std::string s = "abacaba";
  std::vector<size_t> expected = {0, 0, 1, 0, 1, 2, 3};


  std::vector<size_t> given = prefix_function(s);
  
  ASSERT_EQ(expected, given);
}

TEST(StringsTestSuite, kmpAndPrefix_functionEquality) {
  std::string s = "abacabacabadabadatadaba";

  std::vector<size_t> prefix_result = prefix_function(s);
  std::vector<size_t> kmp_result = kmp(s);
  
  ASSERT_EQ(prefix_result, kmp_result);
}

TEST(StringsTestSuite, prefix_functionSubstrings) {
  std::string s = "abacabacabadaba";
  std::string sub = "aba";
  std::vector<size_t> subsExpected = {0, 4, 8, 12};

  std::vector<size_t> subsGiven = kmp_subs(s, sub);

  ASSERT_EQ(subsExpected, subsGiven);
}

