#include <iostream>

using ll = long long ;
using ull = unsigned long long ;
int main() {
  ull N, p, M;

  std::cin >> N >> p >> M;
  
  ull *values = new ull[N];

  for(size_t i = 0, currentPi = 1; i < N; ++i) {
    values[i] = currentPi;
    currentPi *= p;
  }

  ull * counts = new ull[N] {0};

  for(size_t i = N - 1; i > 0; --i) {
    counts[i] = M / values[i];
    M -= counts[i] * values[i];
  }

  counts[0] = M;

  for(size_t i = 0; i < N; ++i)
    std::cout << counts[i] << '\n';

  delete [] values;
  delete [] counts;
  return 0;
}
