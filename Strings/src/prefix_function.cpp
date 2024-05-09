#include "prefix_function.hpp"
#include "z_string.hpp"
#include <cstdint>
#include <vector>


std::vector<size_t> prefix_function(std::string s) {
  std::vector<size_t> z = z_string(s);
  std::vector<size_t> result(s.size(), 0);

  for(int64_t i = 1; i < s.size(); ++i) {
    for(int64_t j = z[i] - 1; j >= 0; --j) {
      if(result[i + j] > 0)
        break;
      else
        result[i + j] = j + 1;
    }
  }

  return result;
}


std::vector<size_t> kmp(std::string s) {
  std::vector<size_t> result(s.size(), 0);
  for(size_t i = 1; i < s.size(); ++i) {
    size_t j = result[i - 1];
    while(j > 0 && s[j] != s[i])
      j = result[j - 1];

    if(s[j] == s[i])
      ++j;

    result[i] = j;
  }

  return result;
}

std::vector<size_t> kmp_subs(std::string s, std::string sub, char sep) {
  std::string concatenated = sub + sep + s;
  std::vector<size_t> prefix_result = prefix_function(concatenated);

  std::vector<size_t> result;

  for (size_t i = 0; i < prefix_result.size(); ++i) {
    if(prefix_result[i] == sub.size()) {
      result.push_back(i - 2*sub.size());
    }
  }

  return result;
}

