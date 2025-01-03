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
  auto actionPacks = processDiff(file1, file2);
  for(auto& pack : actionPacks) {
    printPack(std::cout, pack, file1, file2);
  }
  
  return 0;
}

