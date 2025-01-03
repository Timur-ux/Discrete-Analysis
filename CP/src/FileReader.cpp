#include "FileReader.hpp"
#include "flags.hpp"
#include <algorithm>
#include <cctype>
#include <iterator>
#include <string>

bool isSpace(char c) {
  return (c == ' ') || (c == '\n') || (c == '\t');
}

void processIgnoreCase(std::string &s) {
  std::transform(std::begin(s), std::end(s), std::begin(s),
                 [](char c) { return tolower(c); });
}

std::string processIgnoreTabExpansion(const std::string &s,
                                      size_t tabwidth = 2) {
  std::string res = "";
  std::string tab = "";
  for (size_t i = 0; i < tabwidth; ++i) {
    tab += ' ';
  }

  for (auto &c : s) {
    if (c == '\t') {
      res += tab;
      continue;
    }
    res.push_back(c);
  }
  return res;
}

std::string processIgnoreTrailingSpace(const std::string &s) {
  if(s == "")
    return "";
  std::string res = "";
  long long tail = s.size() - 1;
  while (tail>=0 && isSpace(s[tail])) {
    --tail;
  }
  for (size_t i = 0; i < tail; ++i) {
    res += s[i];
  }

  return res;
}

std::string processIgnoreSpaceChange(const std::string &s) {
  std::string res = "";
  char cPrev = '\0';
  for (auto &c : s) {
    if (isSpace(c) && isSpace(cPrev)) {
      cPrev = c;
      continue;
    }
    res.push_back(c);
    cPrev = c;
  }
  return res;
}

std::string processIgnoreAllSpace(const std::string &s) {
  std::string res = "";
  for (auto &c : s) {
    if (isSpace(c)) {
      continue;
    }
    res.push_back(c);
  }
  return res;
}

std::string processFlags(std::string s) {
  if (flags::ignore_case)
    processIgnoreCase(s);
  if (flags::ignore_all_space) {
    s = processIgnoreAllSpace(s);
  } else {
  }

  return s;
}

FileReader::FileReader(std::string fileName) {
  file.open(fileName);
  std::string line;
  while (std::getline(file, line)) {
    if (line == "" && flags::ignore_black_lines)
      continue;

    if (flags::ignore_case)
      processIgnoreCase(line);

    if (flags::ignore_all_space)
      line = processIgnoreAllSpace(line);
    else if (flags::ignore_space_change)
      line = processIgnoreSpaceChange(line);
    else {
      if (flags::ignore_tab_expansion)
        line = processIgnoreTabExpansion(line);
      if (flags::ignore_trailing_space) 
        line = processIgnoreTrailingSpace(line);
    }

    if(flags::ignore_all_space && line == "") {
      continue;
    }
    buffer.push_back(line);
  }
}

std::string &FileReader::operator[](size_t i) { return buffer.at(i); }
size_t FileReader::size() { return buffer.size(); }
