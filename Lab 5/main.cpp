#include "Aho_Korasik.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

void printOccurrences(occurences &storage) {
  std::cout << "OCCURENCES: " << std::endl;
  for (auto &item : storage) {
    std::cout << item.first << " : count = " << item.second.count << ",";
    for (auto &[pos, wordCount] : item.second.matchings) {
      std::cout << "{ Index = " << pos << ", word count = " << wordCount
                << "}; ";
    }
    std::cout << std::endl;
  }
}

struct Symbol {
  std::string word;
  size_t lineCount;
  size_t wordCount;
};

void checkMask(std::vector<std::string> &patterns,
               std::vector<Symbol> &words,
               occurences &matchings) {
  size_t shift = 0;
  while (shift < patterns.size() && patterns[shift] == "?")
    ++shift;
  if(shift == patterns.size()) {
    for(size_t i = 0; i <= words.size() - shift; ++i) {
      std::cout << words[i].lineCount << ", " << words[i].wordCount << '\n';
    }
    return;
  }

  for (auto &[pos, cnt] : matchings[patterns[shift]].matchings) {
    if (cnt <= shift) // skip if pattern mask's left border less than zero
      continue;
    bool isGoodMask = true;
    for (size_t i = shift; i < patterns.size(); ++i) {
        if ((cnt - 1 + i - shift >= words.size()) || (patterns[i] != "?" && words[cnt - 1 + i - shift].word != patterns[i])) {
          isGoodMask = false;
          break;
      }
    }
    if (isGoodMask)
      std::cout << words[cnt - 1 - shift].lineCount << ", "
                << words[cnt - 1 - shift].wordCount << '\n';
  }
}

void toLower(std::string &s) {
  for(char & c : s)
    c = tolower(c);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  std::vector<std::string> patterns;
  std::vector<std::vector<std::string>> patternsWords; // words pattern consist of
  std::string text = "", word, line;

  std::getline(std::cin, line);
  toLower(line);
  std::stringstream ss(line);

  while (ss >> word)
    patterns.push_back(word);

  std::vector<Symbol> words;
  size_t lineCount = 0;
  while (std::getline(std::cin, line)) {
    toLower(line);
    ++lineCount;

    std::stringstream ss(line);
    size_t wordCount = 0;

    while (ss >> word) {
      text += word + " ";
      ++wordCount;
      words.push_back({word, lineCount, wordCount});
    }
  }

  if(patterns.size() > words.size() || patterns.size() == 0) {
    return 0;
  }

  AKTrie akTrie(patterns);
  occurences matchings = akTrie.find(text);

  checkMask(patterns, words, matchings);
  // std::cout << "TEXT: " << text << std::endl;
  // std::cout << "WORDS: " << std::endl;
  // for (Symbol &w : words) {
  //   std::cout << w.word << ' ' << w.lineCount << ' ' << w.wordCount
  //             << std::endl;
  // }
  // printOccurrences(matchings);
  // std::cout << "PATTERNS: ";
  // for (std::string &p : patterns)
  //   std::cout << p << ',';
  // std::cout << std::endl;
  return 0;
}
