#ifndef DIFF_PROCESSER_HPP_
#define DIFF_PROCESSER_HPP_

#include "FileReader.hpp"
#include <iterator>

enum struct ActionType { none, add, remove, change, pass };
struct Action {
  ActionType type_;
  size_t i1, i2; // Lines in file1 and file2
};

struct ActionsPack {
  ActionType type_;
  std::vector<Action> actions;
};

std::vector<ActionsPack> processDiff(FileReader &buffer1, FileReader &buffer2);
void printPack(std::ostream & os, const ActionsPack & pack, FileReader & file1, FileReader & file2);

#endif // !DIFF_PROCESSER_HPP_
