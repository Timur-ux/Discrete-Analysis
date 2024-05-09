#include "z_string.hpp"
#include <vector>

std::vector<size_t> z_string(std::string s) {
  int64_t l = 0, r = 0;
  int64_t i = 1;

  std::vector<size_t> z(s.size(), 0);

  for(int64_t i = 1; i < s.size(); ++i) {
    z[i] = std::max((int64_t)0, std::min(r - i, (int64_t)z[i - l]));
    while(i + z[i] < s.size() && s[z[i]] == s[i + z[i]])
      ++z[i];

    if(i + z[i] > r) {
      r = i + z[i];
      l = i;
    }
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
