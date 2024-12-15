#include "../include/utils.hpp"
#include "flags.hpp"
#include <stdexcept>


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
