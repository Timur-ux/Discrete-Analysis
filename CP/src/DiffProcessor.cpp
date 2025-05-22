#include "../include/utils.hpp"
#include "DiffProcesser.hpp"
#include "FileReader.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <limits.h>
#include <vector>

std::vector<std::vector<size_t>> diffDP(FileReader &buffer1,
                                        FileReader &buffer2) {
  std::vector<std::vector<size_t>> levis(
      buffer1.size() + 1, std::vector<size_t>(buffer2.size() + 1, 100));
  levis[0][0] = 0;

  for (size_t i = 0; i < buffer1.size(); ++i) {
    levis[i + 1][0] = buffer1[i].size();
  }
  for (size_t j = 0; j < buffer2.size(); ++j) {
    levis[0][j + 1] = buffer2[j].size();
  }

  for (size_t i = 0; i < buffer1.size(); ++i) {
    for (size_t j = 0; j < buffer2.size(); ++j) {
      levis[i + 1][j + 1] = levis[i][j];
      size_t addLineDiff = levis[i][j + 1];
      size_t deleteLineDiff = levis[i + 1][j];

      if (addLineDiff < levis[i + 1][j + 1]) {
        levis[i + 1][j + 1] = addLineDiff;
      }
      if (deleteLineDiff < levis[i + 1][j + 1]) {
        levis[i + 1][j + 1] = deleteLineDiff;
      }

      levis[i + 1][j + 1] += levinshtain(buffer1[i], buffer2[j]);
    }
  }

  return levis;
}

template <typename T> T min(T arg) { return arg; }
template <typename First, typename... Tail> First min(First arg, Tail... args) {
  First tailMin = min(args...);
  if (tailMin < arg) {
    return tailMin;
  }
  return arg;
}

template <typename T> void print(T arg) { std::cout << arg; }

template <typename First, typename... Rest>
void print(First arg, Rest... args) {
  std::cout << arg << ' ';
  print(args...);
}

std::vector<Action> restore(std::vector<std::vector<size_t>> &levis) {
  std::vector<Action> result;
  size_t i1 = levis.size() - 1, i2 = levis[0].size() - 1;

  while (i1 != 0 || i2 != 0) {

    size_t changeVal = INT_MAX, addVal = INT_MAX, removeVal = INT_MAX;
    if (i1 > 0 && i2 > 0) {
      changeVal = levis[i1][i2] - levis[i1 - 1][i2 - 1];
    }
    if (i2 > 0) {
      addVal = levis[i1][i2] - levis[i1][i2 - 1];
    }
    if (i1 > 0) {
      removeVal = levis[i1][i2] - levis[i1 - 1][i2];
    }

    size_t minVal = min(changeVal, addVal, removeVal);

    if (changeVal == minVal) {
      if (changeVal == 0) {
        result.push_back({ActionType::pass, i1, i2});
      } else {
        result.push_back({ActionType::change, i1, i2});
      }
      --i1, --i2;
    } else if (addVal == minVal) {
      result.push_back({ActionType::add, i1, i2});
      --i2;
    } else if (removeVal == minVal) {
      result.push_back({ActionType::remove, i1, i2});
      --i1;
    } else {
      print("Error: undefined min val:", minVal, "\naddVal:", addVal,
            "\nremoveVal:", removeVal, "\nchangeVal:", changeVal, '\n');
    }
  }

  std::reverse(std::begin(result), std::end(result));
  return result;
}

std::vector<ActionsPack> compress(std::vector<Action> &actions) {
  if (actions.size() == 0) {
    return {};
  }
  std::vector<ActionsPack> result;
  result.push_back({actions[0].type_, {actions[0]}});
  for (size_t i = 1; i < actions.size(); ++i) {
    if (actions[i].type_ == result[result.size() - 1].type_) {
      result[result.size() - 1].actions.push_back(actions[i]);
    } else {
      result.push_back({actions[i].type_, {actions[i]}});
    }
  }

  return result;
}

std::vector<ActionsPack> processDiff(FileReader &f1, FileReader &f2) {
  auto levis = diffDP(f1, f2);
  auto actions = restore(levis);

  return compress(actions);
}

void printPack(std::ostream &os, const ActionsPack &pack, FileReader &file1,
               FileReader &file2) {
  if (pack.type_ == ActionType::none) {
    os << "Undefined type, skip...\n";
    return;
  }

  if (pack.type_ == ActionType::pass) {
    return;
  }

  if (pack.type_ == ActionType::change) {
    os << pack.actions[0].i1;
    if (pack.actions.size() > 1) {
      os << ',' << pack.actions[pack.actions.size() - 1].i1 << 'c'
         << pack.actions[0].i2 << ','
         << pack.actions[pack.actions.size() - 1].i2 << '\n';
    } else {
      os << 'c' << pack.actions[0].i2 << '\n';
    }

    for (auto &elem : pack.actions) {
      os << "< " << file1[elem.i1-1] << '\n';
    }
    os << "---\n";
    for (auto &elem : pack.actions) {
      os << "> " << file2[elem.i2-1] << '\n';
    }
  }

  if (pack.type_ == ActionType::add) {
    os << pack.actions[0].i1 << 'a' << pack.actions[0].i2 << ','
       << pack.actions[pack.actions.size() - 1].i2 << '\n';

    for (auto &elem : pack.actions) {
      os << "> " << file2[elem.i2-1] << '\n';
    }
  }

  if (pack.type_ == ActionType::remove) {
    os << pack.actions[0].i1 << ',' << pack.actions[pack.actions.size() - 1].i1
       << 'd' << pack.actions[0].i2 << '\n';

    for (auto &elem : pack.actions) {
      os << "< " << file1[elem.i1-1] << '\n';
    }
  }
}
