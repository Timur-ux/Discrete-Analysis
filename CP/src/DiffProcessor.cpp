#include "../include/utils.hpp"
#include "DiffProcesser.hpp"
#include "FileChange.hpp"
#include "FileReader.hpp"
#include <cstdio>

enum class ActionType { none, pass, add, remove };

struct ActionData {
  ActionType type;
  size_t i;
  size_t leviValue;
};

void diff(FileReader &buffer1, FileReader &buffer2, size_t i1, size_t i2,
          std::list<ActionData> &current, std::list<ActionData> &optimal) {
  if (buffer1[i1] == buffer2[i2]) {
    current.push_back({ActionType::pass, i1, (--current.end())->leviValue});
    diff(buffer1, buffer2, i1+1, i2+1, current, optimal);
  }
}

FileChanges compress(std::list<ActionData> data);

FileChanges diffProcessor(FileReader &buffer1, FileReader &buffer2) {
  std::list<ActionData> current, optimal;
  current.push_back({ActionType::none, 0, 0});

  diff(buffer1, buffer2, 0, 0, current, optimal);

  return compress(optimal);
}
