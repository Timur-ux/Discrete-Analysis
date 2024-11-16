#include "z_string.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(StringsTestSuite, z_stringBuilding) {
  std::string s = "abacabacabadabadatadaba";
  std::vector<size_t> expected = {0, 0, 1, 0, 7, 0, 1, 0, 3, 0, 1, 0, 3, 0, 1, 0, 1, 0, 1, 0, 3, 0, 1};


  std::vector<size_t> given = z_string(s);
  
  ASSERT_EQ(expected, given);
}

TEST(StringsTestSuite, z_stringSubstrings) {
  std::string s = "abacabacabadabadatadaba";
  std::string sub = "aba";
  std::vector<size_t> subsExpected = {0, 4, 8, 12, 20};

  std::vector<size_t> subsGiven = z_string_subs(s, sub);

  ASSERT_EQ(subsExpected, subsGiven);
}
