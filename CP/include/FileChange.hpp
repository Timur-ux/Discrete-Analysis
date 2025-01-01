#ifndef FILE_CHANGE_HPP_
#define FILE_CHANGE_HPP_

#include "FileReader.hpp"
#include <ostream>

using interval = std::pair<long long, long long>;

class FileChange {
public:
  virtual void print(std::ostream &os) = 0;
  virtual ~FileChange() {}
};

class AddLines : public FileChange {
  FileReader &bufferFrom_;
  interval lines_;
  long long to_;
public:
  AddLines(FileReader &bufferFrom, interval lines, long long to)
    : bufferFrom_(bufferFrom), lines_(lines), to_(to) {};

  void print(std::ostream & os) override {
    os << to_ << 'a' << lines_.first << ',' << lines_.second << std::endl;
    for(size_t i = lines_.first; i <= lines_.second; ++i) {
      os << '>' << bufferFrom_[i] << '\n';
    }
  }
};

class DeleteLines : public FileChange {
private:
  FileReader &bufferFrom_;
  interval lines_;
  long long from_;
public:
  DeleteLines(FileReader &bufferFrom, interval lines, long long from)
    : bufferFrom_(bufferFrom), lines_(lines), from_(from) {};

  void print(std::ostream & os) override {
    os << lines_.first << ',' << lines_.second << 'd' << from_ << std::endl;
    for(size_t i = lines_.first; i <= lines_.second; ++i) {
      os << '<' << bufferFrom_[i] << '\n';
    }
  }
  
};

#endif // !FILE_CHANGE_HPP_
