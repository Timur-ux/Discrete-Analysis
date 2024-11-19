#include "../utils/VectorView.hpp"
#include <gtest/gtest.h>
#include <memory>

TEST(UtilsTestSuite, vectorViewAssign) {

  std::shared_ptr<std::vector<int>> data = std::make_shared<std::vector<int>>();
  for(int i = 0; i < 10; ++i)
    data->push_back(i);

  view::vector<int> vview(*data, 0, 2);
  view::vector<int> vview2(*data, 5, 8);

  for(size_t i = 0; i < vview.size(); ++i) {
    EXPECT_EQ(vview[i], data->operator[](i + 0));
  }

  for(size_t i = 0; i < vview2.size(); ++i) {
    EXPECT_EQ(vview2[i], data->operator[](i + 5));
  }
}

TEST(UtilsTestSuite, reverseVectorView) {

  std::shared_ptr<std::vector<int>> data = std::make_shared<std::vector<int>>();
  for(int i = 0; i < 10; ++i)
    data->push_back(i);

  view::vector<int> vview(*data, 0, data->size());
  view::reverseVector<int> vview2(vview);

  for(size_t i = 0; i < vview.size(); ++i) {
    EXPECT_EQ(vview2[i], data->operator[](data->size() - i - 1));
  }

  for(size_t i = 0; i < vview2.size(); ++i) {
    EXPECT_EQ(vview2[i], vview[vview.size() - 1 - i]);
  }
}
