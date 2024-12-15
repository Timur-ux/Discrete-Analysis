#include <algorithm>
#include <ios>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using Graph = std::vector<std::vector<long long int>>;

bool dfs(Graph &graph, std::vector<bool> &used,
         std::vector<long long int> &matchings, long long int v) {
  if (used[v])
    return false;

  used[v] = true;
  for (auto &translation : graph[v])
    if ((matchings[translation] == -1 ||
                               dfs(graph, used, matchings, translation))) {
      matchings[translation] = v;
      return true;
    }

  return false;
}

using Edge = std::pair<long long int, long long int>;

enum class Side {
  none = -1,
  left,
  right
};

std::vector<Side> divide(Graph & graph) {
  std::vector<bool> used(graph.size(), false);
  std::vector<Side> result(graph.size(), Side::none);
  std::queue<int> vertexes;
  vertexes.push(0);
  result[0] = Side::left;
  used[0] = true;

  while(!vertexes.empty()) {
    int v = vertexes.front();
    vertexes.pop();
    
    Side opposite = result[v] == Side::left ? Side::right : Side::left;
    for(auto& to : graph[v]) {
      if(!used[to]) {
        used[to] = true;
        result[to] = opposite;
        vertexes.push(to);
      }
    }
  }

  return result;
}



std::vector<long long int> kune(Graph &graph) {
  std::vector<long long int> matchings(graph.size(), -1);
  // // Разделим на доли
  // std::vector<Side> sides = divide(graph);
  // std::vector<int> leftSideVertexes;
  // for(size_t i = 0; i < sides.size(); ++i) {
  //   if(sides[i] == Side::left) {
  //     leftSideVertexes.push_back(i);
  //   }
  // }
  
  // Теперь сам алгоритм Куна
  for (size_t i = 0; i < graph.size(); ++i) {
    std::vector<bool> used(graph.size(), false);
    dfs(graph, used, matchings, i);
  }

  return matchings;
}


template <typename T, typename V>
std::ostream &operator<<(std::ostream &os, const std::pair<T, V> &pair);
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  for (const auto &el : v)
    os << el << ' ';
  return os;
}

template <typename T, typename V>
std::ostream &operator<<(std::ostream &os, const std::pair<T, V> &pair) {
  os << "{ " << pair.first << ' ' << pair.second << " }";
  return os;
}

int main() {
  std::cin.tie(0);
  std::cout.tie(0);

  std::ios::sync_with_stdio(false);

  long long int n, m;

  std::cin >> n >> m;
  Graph graph(n);
  long long int u, v;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> u >> v;
    graph[u - 1].push_back(v-1);
  }

  std::vector<Edge> edges;

  std::vector<bool> used(graph.size(), false);
  std::vector<long long int> matchings = kune(graph);
  // std::cout << matchings << '\n';
  for(size_t i = 0; i < matchings.size(); ++i) {
    if(matchings[i] != -1 && (!used[i] && !used[matchings[i]])) {
      edges.push_back({i, matchings[i]});
      used[i] = true;
      used[matchings[i]] = true;
    }
  }

  for(auto& p : edges) {
    if(p.first > p.second) {
      std::swap(p.first, p.second);
    }
  }

  std::cout << edges.size() << '\n';
  for(auto& p : edges) {
    std::cout << p.first +1 << ' ' << p.second + 1 << std::endl;
  }
  
  return 0;
}
