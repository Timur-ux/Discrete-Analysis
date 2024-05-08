#include "z_string.hpp"
#include <vector>

std::vector<size_t> z_string(std::string s) {
  size_t l = 0, r = 0;
  size_t i = 1;

  std::vector<size_t> z(s.size(), 0);

  while(i < s.size()) {
    if(i > r) {
      while(i + z[i] < s.size() && s[z[i]] == s[i + z[i]])
        ++z[i];

      r = i + z[i];
      l = i;
    } else {
      z[i] = std::min(z[i - l], r - l + 1);

      while(i + z[i] < s.size() && s[z[i]] == s[i + z[i]])
        ++z[i];

      if(i + z[i] > r) {
        r = i + z[i];
        l = i;
      }
    }
    ++i;
  }

  return z;
}

std::vector<size_t> z_string_subs(std::string s, std::string sub, char sep) {
  std::string concatenated = sub + sep + s;
  std::vector<size_t> z = z_string(concatenated);
  std::vector<size_t> result{};

  for(size_t i = 0; i < z.size(); ++i) {
    if(z[i] == sub.size()) 
      result.push_back(i - sub.size() - 1);
  }
  
  return result;
}
