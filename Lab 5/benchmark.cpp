#include "Aho_Korasik.hpp"
#include "prefix_function.hpp"
#include "z_string.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using tp_t = std::chrono::time_point<std::chrono::system_clock>;
using duration_t = std::chrono::milliseconds;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  std::vector<std::string> patterns;
  size_t patternsSize = 0;
  std::string text;

  std::string line, word;

  std::getline(std::cin, line);
  std::stringstream ss(line);

  while(ss >> word) {
    patterns.push_back(word);
    patternsSize += word.size();
  }

  while(std::cin >> word)
    text += word + " ";

  tp_t start, end;
  duration_t aho_korasikTime, kmpTime, z_functionTime;

  start = std::chrono::system_clock::now();
  AKTrie akTrie(patterns);
  akTrie.find(text);
  end = std::chrono::system_clock::now();

  aho_korasikTime = std::chrono::duration_cast<duration_t>(end - start);

  start = std::chrono::system_clock::now();
  for(auto & pattern: patterns)
    kmp_subs(text, pattern);
  end = std::chrono::system_clock::now();

  kmpTime = std::chrono::duration_cast<duration_t>(end - start);

  start = std::chrono::system_clock::now();
  for(auto & pattern: patterns)
    z_string_subs(text, pattern);
  end = std::chrono::system_clock::now();

  z_functionTime = std::chrono::duration_cast<duration_t>(end - start);

  std::cout << "Pattern's quantity: " << patterns.size() << '\n';
  std::cout << "Overall pattern's size: " << patternsSize << '\n';
  std::cout << "Text size: " << text.size() << '\n';

  std::cout << "Aho-Korasik time: " << aho_korasikTime.count() << '\n';
  std::cout << "KMP time: " << kmpTime.count() << '\n';
  std::cout << "z-function time: " << z_functionTime.count() << '\n';
  return 0;
}
