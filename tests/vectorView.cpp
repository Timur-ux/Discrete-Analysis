#include "../utils/VectorView.hpp"
#include <gtest/gtest.h>
#include <memory>

TEST(UtilsTestSuite, vectorViewAssign) {

  std::shared_ptr<std::vector<int>> data = std::make_shared<std::vector<int>>();
  for(int i = 0; i < 10; ++i)
    data->push_back(i);

  view::vector<int> vview(data, 0, 2);
  view::vector<int> vview2(data, 5, 8);

  for(size_t i = 0; i < vview.size(); ++i) {
    EXPECT_EQ(vview[i], data->operator[](i + 0));
  }

  for(size_t i = 0; i < vview2.size(); ++i) {
    EXPECT_EQ(vview2[i], data->operator[](i + 5));
  }
}

TEST(UtilsTestSuite, vectorViewPair) {

  std::shared_ptr<std::vector<int>> data = std::make_shared<std::vector<int>>();
  for(int i = 0; i < 10; ++i)
    data->push_back(i);

  view::vector<int> vview(data, 0, 2);
  view::vector<int> vview2(data, 5, 8);

  view::vectorPair<int> vpair(vview, vview2);

  for(size_t i = 0; i < vview.size(); ++i) {
    EXPECT_EQ(vpair[i], data->operator[](i + 0));
  }

  for(size_t i = 0; i < vview2.size(); ++i) {
    EXPECT_EQ(vpair[i + vview.size()], data->operator[](i + 5));
  }
}

TEST(UtilsTestSuite, vectorViewPairPair) {

  std::shared_ptr<std::vector<int>> data = std::make_shared<std::vector<int>>();
  for(int i = 0; i < 10; ++i)
    data->push_back(i);

  view::vector<int> vview(data, 0, 2);
  view::vector<int> vview2(data, 5, 8);
  view::vectorPair<int> vpair(vview, vview2);

  view::vector<int> vview3(data, 0, 7);
  view::vectorPair<int> vpair2(vview, vpair);

  for(size_t i = 0; i < vview3.size(); ++i) {
    EXPECT_EQ(vpair2[i], data->operator[](i + 0));
  }

  for(size_t i = 0; i < vview.size(); ++i) {
    EXPECT_EQ(vpair[i + vview3.size()], data->operator[](i + 0));
  }

  for(size_t i = 0; i < vview2.size(); ++i) {
    EXPECT_EQ(vpair[i + vview3.size()], data->operator[](i + 5));
  }
}
