#include "FileReader.hpp"
#include <string>

#ifdef DEBUG
#include <iostream>
#endif // !DEBUG

FileReader::FileReader(std::string fileName) {
  file.open(fileName);
  std::string line;
  while (std::getline(file, line)) {
    buffer.push_back(line);
  }

#ifdef DEBUG
  for (auto &line : buffer) {
    std::cout << line << '\n';
  }
#endif // !DEBUG
}
std::string &FileReader::operator[](size_t i) { return buffer.at(i); }
size_t FileReader::size() { return buffer.size(); }
