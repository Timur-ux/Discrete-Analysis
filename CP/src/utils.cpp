#include "../include/utils.hpp"
#include "flags.hpp"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "Hashberg.hpp"
#include <iostream>


void parseFlags(std::string & s) {
  for(size_t i = 1; i < s.size(); ++i) {
    switch (s[i]) {
      case 'i':
        flags::ignore_case = true;
        break;
      case 'E':
        flags::ignore_tab_expansion = true;
        break;
      case 'Z':
        flags::ignore_trailing_space = true;
        break;
      case 'b':
        flags::ignore_space_change = true;
        break;
      case 'w':
        flags::ignore_all_space = true;
        break;
      case 'B':
        flags::ignore_black_lines = true;
        break;
      case 't':
        flags::expand_tabs_to_spaces_in_output = true;
        break;
      default:
        throw std::invalid_argument("Invalid flags line near" + std::to_string(i) +" index: " + s);
    }
  }
}

std::vector<int> toInt(const std::string &s) {
  std::vector<int> res;
  res.reserve(s.size());
  for(auto& c : s) {
    res.push_back(c);
  }
  
  return res;
}

size_t levinshtain(const std::string &s1, const std::string&s2) {
  if(s1.size() == 0 || s2.size() == 0) {
    return std::max(s1.size(), s2.size());
  }
  
  std::vector<int> lcp = H2_prim(toInt(s1), toInt(s2));
  int d = lcp[lcp.size() - 1];
  return s1.size() + s2.size() - 2 * d;
}
