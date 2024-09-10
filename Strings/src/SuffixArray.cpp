#include "SuffixArray.hpp"
#include "SuffixTrie.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

void SuffixArray::createFromTrie(SuffixTrie::Node *node,
                                 size_t &commonSuffixLen, size_t totalLen) {
  if (node->translations.empty()) {
    suffixIndexes_.push_back(node->suffixStartPos.value());
    LCP_.push_back(commonSuffixLen);
    commonSuffixLen = totalLen;
    return;
  }

  for (auto &[_, child] : node->translations) {
    createFromTrie(child, commonSuffixLen,
                   totalLen + (*node->slice.second - node->slice.first));
  }

  commonSuffixLen -= *node->slice.second - node->slice.first;
}

SuffixArray::SuffixArray(const SuffixTrie &trie) : text_(trie.text_) {
  suffixIndexes_.reserve(trie.text_.size());
  size_t commonSuffixLen = 0;

  std::vector<size_t> newLCP(2 * trie.text_.size());

  createFromTrie(trie.root_, commonSuffixLen, 0);
  LCP_.erase(std::begin(LCP_));

  fillLCP(0, trie.text_.size() - 1, newLCP);

  LCP_ = std::move(newLCP);
  printLCPStructure(0, trie.text_.size() - 1, trie.text_);
}

void SuffixArray::printLCPStructure(size_t L, size_t R,
                                    const std::string &text) {
  size_t curLCP =
      ((R - L == 1) || (R + L) % 2 == 0 ? LCP_[R + L - 1] : LCP_[R + L - 2]);
  std::cout << "L = " << L << ", R = " << R << ", LCP = " << curLCP
            << std::endl;
  for (size_t i = 0; i < curLCP; ++i)
    std::cout << ' ';
  std::cout << "|" << std::endl;
  for (size_t i = suffixIndexes_[L]; i < suffixIndexes_[L] + curLCP + 1; ++i)
    std::cout << text[i];
  std::cout << std::endl;
  for (size_t i = suffixIndexes_[R]; i < suffixIndexes_[R] + curLCP + 1; ++i)
    std::cout << text[i];
  std::cout << std::endl;

  if (R - L == 1)
    return;
  printLCPStructure(L, (R + L) / 2, text);
  printLCPStructure((R + L) / 2, R, text);
}

size_t SuffixArray::fillLCP(size_t L, size_t R, std::vector<size_t> &newLCP) {
  if (R == L)
    throw std::invalid_argument("R and L can't be equal");

  if (R < L)
    std::swap(L, R);

  if (R - L == 1) {
    newLCP[L + R - 1] = LCP_[L];
    return LCP_[L];
  }

  size_t result = std::min(fillLCP(L, (L + R) / 2, newLCP),
                           fillLCP((L + R) / 2, R, newLCP));

  if ((R - L) % 2 == 0)
    newLCP[L + R - 1] = result;
  else
    newLCP[L + R - 2] = result;

  return result;
}

size_t SuffixArray::getLCPAt(size_t L, size_t R) {
  if (R < L)
    std::swap(L, R);
  if ((L + R) % 2 == 0)
    return LCP_[L + R - 1];
  else
    return LCP_[L + R - 2];
}

std::vector<size_t> SuffixArray::find(const std::string &pattern) const {
  std::vector<size_t> result;

  size_t L = 0, R = suffixIndexes_.size() - 1;
  size_t l = 0, r = 0;

  while (L < R) {
    while (std::max(l, r) < pattern.size() &&
           (pattern[l] == text_[suffixIndexes_[L] + l] ||
            pattern[r] == text_[suffixIndexes_[R] + r])) {
      if (pattern[l] == text_[suffixIndexes_[L] + l])
        ++l;
      if (pattern[r] == text_[suffixIndexes_[R] + r])
        ++r;
    }
    if (std::max(l, r) >= pattern.size())
      break;

    size_t mid = (l + r) / 2;
    if (l == r) {
      if (pattern[l] > text_[suffixIndexes_[mid] + l])
        L = mid;
      else
        R = mid;
    } else if (l > r) {
      size_t k = getLCPAt(l, mid);
    }
  }
};
