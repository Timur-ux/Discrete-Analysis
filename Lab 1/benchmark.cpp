#include <iostream>
#include <chrono>
#include <cstdint>

#include "CountingSort.hpp"


using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

int main() {
  sorts::CountingSort sorter;

  std::vector<Item> input, input_stl;
  uint16_t key;
  uint64_t value;
  while (std::cin >> key >> value)
  {
    input.push_back(Item{ key, value });
  }
  input_stl = input;

  std::cout << "Count of lines is " << input.size() << std::endl;

  // Измеряем время работы сортировки подсчётом.
  std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
  sorter.sort(input);
  auto end_ts = std::chrono::system_clock::now();
  uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>(end_ts - start_ts).count();

  // Измеряем время работы stl сортировки.
  start_ts = std::chrono::system_clock::now();
  std::stable_sort(std::begin(input_stl), std::end(input_stl));
  end_ts = std::chrono::system_clock::now();

  uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>(end_ts - start_ts).count();
  std::cout << "Counting sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
  std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
}
