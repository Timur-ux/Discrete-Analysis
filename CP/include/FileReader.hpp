#ifndef FILE_READER_HPP_
#define FILE_READER_HPP_

#include <fstream>
#include <string>
#include <vector>
class FileReader {
private:
  std::ifstream file;
  std::vector<std::string> buffer;
public:
  FileReader(std::string fileName);

  std::string & operator[](size_t i);
  size_t size();
  bool empty() {return size() == 0;}
};

#endif // !FILE_READER_HPP_
