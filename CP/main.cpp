#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "Hashberg.hpp"

int main() {
  std::map<std::string, int> s2i;
  std::map<int, std::string> i2s;

  std::string line1, line2;
  std::getline(std::cin, line1);
  std::getline(std::cin, line2);

  std::vector<int> v1, v2;

  std::istringstream ss1(line1), ss2(line2);
  std::string word;
  while(ss1 >> word) {
    if(s2i.find(word) == s2i.end()) {
      s2i[word] = s2i.size();
      i2s[s2i.size() - 1] = word;
    }

    v1.push_back(s2i[word]);
  }

  while(ss2 >> word) {
    if(s2i.find(word) == s2i.end()) {
      s2i[word] = s2i.size();
      i2s[s2i.size() - 1] = word;
    }

    v2.push_back(s2i[word]);
  }

  std::vector<int> result = H3_prim(v1, v2);

  std::cout << result.size() << std::endl;
  for(int el :result) 
    std::cout << i2s[el] << ' ';
  std::cout << std::endl;

  return 0;
}

