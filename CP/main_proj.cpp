#include "DiffProcesser.hpp"
#include "FileReader.hpp"
#include "./include/utils.hpp"
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
  


  std::cout << argw[argc-2] << ' ' << argw[argc-1]<< std::endl;
  FileReader file1(argw[argc-2]);
  FileReader file2(argw[argc-1]);

  std::cout << "Min dist between file1 and file2: " << diffDP(file1, file2) << std::endl;

  return 0;
}

