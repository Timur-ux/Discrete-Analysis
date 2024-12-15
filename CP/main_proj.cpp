#include "FileReader.hpp"
#include "./include/utils.hpp"
#include "flags.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char * argw[]) {
  if(argc < 3) {
    throw std::invalid_argument("Usage <util> <flags?>* <file1> <file2>");
  }
  for(size_t i = 1; i < argc-2; ++i) {
    if(argw[i][0] != '-')
      throw std::invalid_argument("Flags must begin with -");
    std::string s = argw[i];
    parseFlags(s);
  }
  


  FileReader file1(argw[argc-2]);
  FileReader file2(argw[argc-1]);
  std::cout << flags::ignore_case << '\n' << flags::expand_tabs_to_spaces_in_output << '\n' << flags::ignore_black_lines << '\n' << flags::ignore_all_space << '\n' << flags::ignore_trailing_space << '\n' << flags::ignore_space_change <<'\n' << flags::ignore_tab_expansion <<std::endl;

  return 0;
}

