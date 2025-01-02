#include "DiffProcesser.hpp"
#include "FileReader.hpp"
#include "../include/utils.hpp"
#include <cstdio>
#include <iostream>
#include <limits.h>

size_t diffDP(FileReader &buffer1, FileReader &buffer2) {
  std::vector<std::vector<size_t>> levis(buffer1.size()+1, std::vector<size_t>(buffer2.size() + 1, 100));
  levis[0][0] = 0;

  for (size_t i = 0; i < buffer1.size(); ++i) {
    levis[i + 1][0] = buffer1[i].size();
  }
  for(size_t j = 0; j < buffer2.size(); ++j) {
    levis[0][j+1] = buffer2[j].size();
  }

  for (size_t i = 0; i < buffer1.size(); ++i) {
    for (size_t j = 0; j < buffer2.size(); ++j) {
      levis[i + 1][j + 1] = levis[i][j] ;
      size_t addLineDiff = levis[i][j+1];
      size_t deleteLineDiff = levis[i+1][j];
      
      if(addLineDiff < levis[i+1][j+1]) {
        levis[i+1][j+1] = addLineDiff;
      }
      if(deleteLineDiff < levis[i+1][j+1]) {
        levis[i+1][j+1] = deleteLineDiff;
      }

      levis[i+1][j+1] += levinshtain(buffer1[i], buffer2[j]);
    }
  }

  std::cout << "" << std::endl;
  for(size_t i = 0; i <= buffer1.size(); ++i) {
    for(size_t j = 0; j <= buffer2.size(); ++j) {
      std::cout << levis[i][j] << '\t';
    }
    std::cout << "" << std::endl;
  }
  
  
  return levis[buffer1.size()][buffer2.size()];
}
