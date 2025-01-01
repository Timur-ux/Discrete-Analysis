#ifndef DIFF_PROCESSER_HPP_
#define DIFF_PROCESSER_HPP_

#include "FileChange.hpp"
#include "FileReader.hpp"
#include <list>

// Simple auto deleting items wrapper on list with changes
class FileChanges : public std::list<FileChange*> {
public:
  virtual ~FileChanges() {
    for(auto& elem : *this) {
      delete elem;
    }
  }
};


inline FileChanges diffProcessor(FileReader &buffer1, FileReader &buffer2);

#endif // !DIFF_PROCESSER_HPP_
