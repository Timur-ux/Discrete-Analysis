#ifndef BIN_SEARCH_H_
#define BIN_SEARCH_H_
#include <concepts>
#include <functional>
#include <vector>

template <typename T>
concept TComparable = requires(T a, T b) { a <=> b; };

template <typename T, TComparable TKey>
std::vector<T>::iterator binSearch(std::vector<T> &data, TKey &key,
                                   const std::function<TKey(T&)> keyFetcher) {
  size_t L = 0;
  size_t R = data.size();

  size_t mid = (L + R) / 2;
  while (L < R) {
    if (keyFetcher(data[mid]) >= key) {
      R = mid;
    } else {
      L = mid + 1;
    }
    mid = (L + R) / 2;
  }
  return data.begin() + mid;
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>>
split(std::vector<T> &data, typename std::vector<T>::iterator it) {
  std::pair<std::vector<T>, std::vector<T>> result;

  for(auto curIt = data.begin(); curIt != it; ++curIt) {
    result.first.emplace_back(*curIt);
  }

  for(auto curIt = it; curIt != data.end(); ++curIt) {
    result.second.emplace_back(*curIt);
  }

  return result;
}

#endif // !BIN_SEARCH_H_
