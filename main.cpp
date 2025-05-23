#include <iostream>
#include <map>
#include <sstream>
#include <vector>

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
  std::vector<int> L2 = H2_prim(reverse(slice(x1, i, x1.size())), reverse(x2));

  int j0 = -1;
  int max = -1;
  for(int j = 0; j < L1.size(); ++j) {
    if(L1[j] + L2[x2.size() - 1 - j] > max) {
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

int main() {
  std::map<std::string, int> s2i;
  std::map<int, std::string> i2s;

  std::string line1, line2;
  std::getline(std::cin, line1);
  std::getline(std::cin, line2);

  std::vector<int> v1, v2;

  std::istringstream ss1(line1), ss2(line2);
  std::string word;
  while(ss1 >> word) {
    if(s2i.find(word) == s2i.end()) {
      s2i[word] = s2i.size();
      i2s[s2i.size() - 1] = word;
    }

    v1.push_back(s2i[word]);
  }

  while(ss2 >> word) {
    if(s2i.find(word) == s2i.end()) {
      s2i[word] = s2i.size();
      i2s[s2i.size() - 1] = word;
    }

    v2.push_back(s2i[word]);
  }

  std::vector<int> result = H3_prim(v1, v2);

  std::cout << result.size() << std::endl;
  for(int el :result) 
    std::cout << i2s[el] << ' ';
  std::cout << std::endl;

  return 0;
}

