#include "compact_trie.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(CompactTrie, insert) {
  std::vector<std::string> data = {
    "",
    "roman",
    "romania",
    "rum",
    "rune",
    "rominatia"
  };

  CompactTrie trie;

  for (auto &s : data) {
    trie.insert(s);
  }

  for (auto &s : data) {
    EXPECT_TRUE(trie.find(s));
  }

  std::string outherString = "sdlfkjssdfffffflkfjslk";
  EXPECT_FALSE(trie.find(outherString));
}

TEST(CompactTrie, insertInPlace) {
  std::vector<std::string> data = {
    "",
    "roman",
    "romania",
    "rum",
    "rune",
    "rominatia"
  };

  CompactTrie trie(data);

  for (auto &s : data) {
    EXPECT_TRUE(trie.find(s));
  }

  std::string outherString = "sdlfkjssdfffffflkfjslk";
  EXPECT_FALSE(trie.find(outherString));
}

TEST(CompactTrie, delete) {
  std::vector<std::string> data = {
    "",
    "roman",
    "romania",
    "rum",
    "rune",
    "rominatia"
  };
  CompactTrie trie(data);

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
