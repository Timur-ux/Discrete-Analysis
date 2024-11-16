#include "../utils/utils.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

TEST(UtilsTestSuite, binSearch) {
  const int n = 10000;
  std::vector<int> data(n);

  for (size_t i = 0; i < 10; i++) {
    for (int j = 0; j < n; ++j) {
      data[j] = rand();
    }

    std::sort(data.begin(), data.end());

    for (int j = 0; j < n; ++j) {
      int searchResult =
          *binSearch<int, int>(data, data[i], [](int item) { return item; });
      ASSERT_EQ(data[i], searchResult);
    }
  }
}

TEST(UtilsTestSuite, binSearchMissed) {
  std::vector<int> data = {1, 2, 3, 5};
  int itemToFind = 4;

  auto it =
      binSearch<int, int>(data, itemToFind, [](int item) { return item; });

  ASSERT_EQ(*it, 5);
  --it;
  ASSERT_EQ(*it, 3);
}

TEST(UtilsTestSuite, split) {
  std::vector<int> data{1, 2, 4, 5};
  int numToSplit = 3;
  std::pair<std::vector<int>, std::vector<int>> expected = {{1, 2}, {4, 5}};

  auto given = split(data, binSearch<int, int>(data, numToSplit,
                                               [](int item) { return item; }));

  ASSERT_EQ(expected, given);
}
