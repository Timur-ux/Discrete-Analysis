#include "Hashberg.hpp"
#include <gtest/gtest.h>

TEST(OpaqueStringComparision, HashbergLCPLength) {
  std::vector<int> v1{1, 6, 23, 5, 10, 6};
  std::vector<int> v2{5, 1, 10, 6, 6, 10, 11};

  // std::vector<int> lcpLine = H2({v2, 0, v2.size()}, {v1, 0, v1.size()});
  std::vector<int> lcpLine = H2_prim(v2, v1);
  std::vector<int> expected = {0, 1, 2, 2, 2, 3, 3};


  EXPECT_EQ(lcpLine.size(), expected.size());
  for(size_t i = 0; i < lcpLine.size(); ++i) {
    EXPECT_EQ(lcpLine[i], expected[i]);
  }
}

TEST(OpaqueStringComparision, HashbergLCPConstruct) {
  std::vector<int> v2{1, 6, 23, 5, 10, 6};
  std::vector<int> v1{5, 1, 10, 6, 6, 10, 11};

  // std::vector<int> lcp = H3({v1, 0, v1.size()}, {v2, 0, v2.size()});
  std::vector<int> lcp = H3_prim(v1, v2);
  std::vector<int> expected{1, 6, 10};

  std::cout << "lcp: ";
  for(int c : lcp) 
    std::cout << c << ' ';
  std::cout << '\n';
  
  EXPECT_EQ(lcp.size(), expected.size());
  for(size_t i = 0; i < lcp.size(); ++i) {
    EXPECT_EQ(lcp[i], expected[i]);
  }
}

TEST(OpaqueStringComparision, HashbergLCPConstruct2) {
  std::vector<int> v1{'s', 't', 'r', 'e', 's', 's'};
  std::vector<int> v2{'r', 'e', 's', 't', 's'};

  // std::vector<int> lcp = H3({v1, 0, v1.size()}, {v2, 0, v2.size()});
  std::vector<int> lcp = H3_prim(v1, v2);
  std::vector<int> expected{'r', 'e', 's', 's'};
  std::cout << "lcp: ";
  for(char c : lcp) 
    std::cout << c;
  std::cout << '\n';

  
  EXPECT_EQ(lcp.size(), expected.size());
  for(size_t i = 0; i < lcp.size(); ++i) {
    EXPECT_EQ(lcp[i], expected[i]);
  }
}
