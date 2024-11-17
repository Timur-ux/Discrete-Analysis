#include "Hashberg.hpp"
#include "VectorView.hpp"
#include <iostream>

template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> seq) {
  for (size_t i = 0; i < seq.size(); ++i)
    std::cout << seq[i] << ' ';
  // std::cout << char(seq[i]) << ' ';

  return os;
}
template <typename T>
std::ostream &operator<<(std::ostream &os, view::vector<T> seq) {
  for (size_t i = 0; i < seq.size(); ++i)
    std::cout << seq[i] << ' ';
  // std::cout << char(seq[i]) << ' ';

  return os;
}

std::vector<int> H2(const view::vector<int> x1, const view::vector<int> x2) {
  LCPMatrix gamma(2, std::vector<int>(x2.size() + 1, 0));
  size_t i_ = 0;
  for (size_t i = 0; i < x1.size(); ++i) {
    i_ = 1 - i_;
    for (size_t j = 0; j < x2.size(); ++j) {
      if (x1[i] == x2[j])
        gamma[i_][j + 1] = gamma[1 - i_][j] + 1;
      else
        gamma[i_][j + 1] = std::max(gamma[1 - i_][j + 1], gamma[i_][j]);
    }
  }

  return gamma[i_];
}

template <typename T> bool contains(const T x, const view::vector<T> &v) {
  for (size_t i = 0; i < v.size(); ++i)
    if (v[i] == x)
      return true;

  return false;
}

std::vector<int> H3(const view::vector<int> x1, const view::vector<int> x2) {
  // Base of recursion
  if (std::min(x1.size(), x2.size()) == 0) {
    return {};
  } else if (x1.size() == 1) {
    if (contains(x1[0], x2))
      return {x1[0]};
    return {};
  }

  // Body of recursion
  size_t i = x1.size() / 2;
  std::vector<int> gamma1 = H2(view::vector(x1, 0, i), x2);
  std::vector<int> gamma2 =
      H2(view::reverseVector<int>(x1, i, x1.size()), view::reverseVector(x2));

  long long j0 = -1;
  size_t gammaMax = gamma2[0];
  for (size_t j = 0; j < x2.size(); ++j) {
    if (gamma1[j] + gamma2[x2.size() - (j + 1)] > gammaMax) {
      gammaMax = gamma1[j] + gamma2[x2.size() - (j + 1)];
      j0 = j;
    }
  }

  std::vector<int> g1 =
      H3(view::vector<int>(x1, 0, i), view::vector<int>(x2, 0, j0));
  std::vector<int> g2 = H3(view::vector<int>(x1, i, x1.size()),
                           view::vector<int>(x2, j0, x2.size()));

  for (auto &val : g2)
    g1.push_back(val);

  return g1;
}

std::vector<int> H2_prim(std::vector<int> x1, std::vector<int> x2) {
  std::vector<std::vector<int>> buf(2, std::vector<int>(x2.size() + 1, 0));

  int i_ = 0;
  for (int i = 0; i < x1.size(); ++i) {
    i_ = 1 - i_;
    for (int j = 0; j < x2.size(); ++j) {
      if (x1[i] == x2[j])
        buf[i_][j + 1] = buf[1 - i_][j] + 1;
      else
        buf[i_][j + 1] = std::max(buf[1 - i_][j + 1], buf[i_][j]);
    }
  }

  return buf[i_];
}

std::vector<int> reverse(std::vector<int> v) {
  for (size_t i = 0; i < v.size() / 2; ++i)
    std::swap(v[i], v[v.size() - 1 - i]);

  return v;
}

std::vector<int> slice(std::vector<int> v, int start, int end) {
  std::vector<int> res;
  for(size_t i = start; i < end; ++i) {
    res.push_back(v[i]);
  }

  return res;
}

std::vector<int> H3_prim(std::vector<int> x1, std::vector<int> x2) {
  if(std::min(x1.size(), x2.size()) == 0)
    return {};
  else if(x1.size() == 1) {
    for(auto & val: x2)
      if(val == x1[0])
        return {x1[0]};
    return {};
  }

  int i = x1.size() / 2;
  std::vector<int> L1 = H2_prim(slice(x1, 0, i), x2);
  std::vector<int> L2 = reverse(H2_prim(reverse(slice(x1, i, x1.size())), reverse(x2)));

  int j0 = -1;
  int max = -1;
  for(int j = 0; j < L1.size(); ++j) {
    if(L1[j] + L2[j] > max) {
      max = L1[j] + L2[j];
      j0 = j;
    }
  }
  // std::cout << L1 << '\n' << L2 << '\n' << max << ' ' << j0 << '\n';

  std::vector<int> g1 = H3_prim(slice(x1, 0, i), slice(x2, 0, j0));
  std::vector<int> g2 = H3_prim(slice(x1, i, x1.size()), slice(x2, j0, x2.size()));

  for(auto el : g2) 
    g1.push_back(el);

  return g1;
}
