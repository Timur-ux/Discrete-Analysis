#include "trie.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(Trie, insert) {
  std::vector<std::string> data = {
      "123456",
      "sdflkjslfj12",
      "aaaaaaaaaxxxxxxxxxx",
  };

  Trie trie;

  for (auto &s : data) {
    trie.insert(s);
  }

  for (auto &s : data) {
    EXPECT_TRUE(trie.find(s));
  }

  std::string outherString = "sdlfkjssdfffffflkfjslk";
  EXPECT_FALSE(trie.find(outherString));
}

TEST(Trie, insertInPlace) {
  std::vector<std::string> data = {
      "123456",
      "sdflkjslfj12",
      "aaaaaaaaaxxxxxxxxxx",
  };

  Trie trie(data);

  for (auto &s : data) {
    EXPECT_TRUE(trie.find(s));
  }

  std::string outherString = "sdlfkjssdfffffflkfjslk";
  EXPECT_FALSE(trie.find(outherString));
}

TEST(Trie, delete) {
  std::vector<std::string> data = {
      "", "roman", "router", "routine", "raise", "root",
  };
  Trie trie(data);

  for (int i = 1; i < data.size(); i += 2) {
    trie.remove(data[i]);
  }

  for (int i = 1; i < data.size(); i += 2) {
    EXPECT_FALSE(trie.find(data[i]));
  }
  for (int i = 0; i < data.size(); i += 2) {
    EXPECT_TRUE(trie.find(data[i]));
  }
}
