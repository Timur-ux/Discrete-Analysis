#include <iostream>

using ull = unsigned long long;
#define MAX_LEN 100

size_t cache[MAX_LEN][MAX_LEN]{0};

// Считает варианты, если длина текущего полиндрома нечетная
size_t calcOdd(std::string &s, size_t l, size_t r);

// Считает варианты, если длина текущего полиндрома четная
size_t calcEven(std::string &s, size_t l, size_t r) {
  if(cache[l][r] != 0) {
    return cache[l][r];
  }

  size_t result = 0;
  for(size_t i = l; i < r; ++i) {
    result += 1 + calcOdd(s, i, r);
  }

  cache[l][r] = result;
  return result;
}

size_t calcOdd(std::string &s, size_t l, size_t r) {
  if(cache[l][r] != 0) {
    return cache[l][r];
  }
  size_t result = 0;
  for(size_t i = l+1; i < r; ++i) {
    if(s[i] == s[l]) {
      result += 1 + calcEven(s, l+1, i);
    }
  }

  // cache[l][r] = result;
  return result;
}


int main() {

  std::string s;
  std::cin >> s;

  ull result = calcEven(s, 0, s.size());

  std::cout << result << std::endl;

  return 0;
}
