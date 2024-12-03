#include <algorithm>
#include <ios>
#include <iostream>
#include <vector>

using Graph = std::vector<std::vector<int>>;

bool dfs(Graph &graph, std::vector<bool> &used, std::vector<int> &matchings,
         int v) {
  if (used[v])
    return false;

  used[v] = true;
  for (auto &translation : graph[v])
    if (matchings[translation] == -1 ||
        dfs(graph, used, matchings, translation)) {
      matchings[translation] = v;
      return true;
    }

  return false;
}

using Edge = std::pair<int, int>;

std::vector<int> kune(Graph &graph) {
  std::vector<int> matchings(graph.size(), -1);
  // Теперь сам алгоритм Куна
  for (size_t i = 0; i < graph.size(); ++i) {
    std::vector<bool> used(graph.size(), false);
    dfs(graph, used, matchings, i);
  }

  return matchings;
}

bool dfs2(Graph & graph, std::vector<bool> &used, int v, std::vector<Edge> &result, int vNum = 1) {
  if(used[v])
    return false;
  used[v] = true;

  for(int to : graph[v]) {
    if(dfs2(graph, used, to, result, vNum + 1)) {
      if(vNum % 2 == 1) {
        result.push_back({v, to});
      }
    }
  }
  return true;
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

  int n, m;

  std::cin >> n >> m;
  Graph graph(n);
  int u, v;
  while(std::cin >> u >> v) {
    graph[u-1].push_back(v-1);
    graph[v-1].push_back(u-1);
  }

  for (auto &row : graph)
    std::sort(row.begin(), row.end());

  // if(m == 1) {
  //   std::cout << 1 << '\n';
  //   for(size_t i = 0; i < graph.size(); ++i)
  //     if(graph[i].size() > 0) {
  //       std::cout << std::min((int)i, graph[i][0]) + 1 << ' ' << std::max((int)i, graph[i][0]) + 1 << '\n';
  //       return 0;
  //     }
  // }

  std::vector<int> matchings = kune(graph);
  std::vector<Edge> edges;
  for (size_t i = 0; i < matchings.size(); ++i)
    if (matchings[i] != -1)
      edges.push_back({i, matchings[i]});


  for (auto &p : edges)
    if (p.first > p.second)
      std::swap(p.first, p.second);

  std::sort(std::begin(edges), std::end(edges),
            [](const Edge &a, const Edge &b) { return a.first < b.first; });
  if(edges.size() == 0) {
    std::cout << 0 << '\n';
    return 0;
  }

  std::vector<Edge> edgesNoCopies{edges[0]};
  for(size_t i = 1; i < edges.size(); ++i) {
    if(edges[i].first != edges[i-1].first)
      edgesNoCopies.push_back(edges[i]);
  }

  // Строим граф из цепи и ищем его начало
  Graph graph2(graph.size());
  std::vector<int> occurences(graph.size(), 0);
  for (auto &edge : edgesNoCopies) {
    graph2[edge.first].push_back(edge.second);
    graph2[edge.second].push_back(edge.first);
    ++occurences[edge.first];
    ++occurences[edge.second];
  }
  size_t pathEnd = 0;
  while((pathEnd < occurences.size()) && (occurences[pathEnd] != 1)) {
    ++pathEnd;
  }

  // Теперь надо из найденной концевой вершины запустить dfs и брать только нечетные ребра в ответ 1, 3, 5 и т.д.

  std::vector<Edge> result;
  std::vector<bool> used(graph.size(), false);
  dfs2(graph2, used, pathEnd, result, 1);

  // Посортируем все на всякий случай
  for (auto &p : result)
    if (p.first > p.second)
      std::swap(p.first, p.second);

  std::sort(std::begin(result), std::end(result),
            [](const Edge &a, const Edge &b) { return a.first < b.first; });


  std::vector<Edge> mustResult{result[0]};
  for(size_t i = 1; i < result.size(); ++i)
    if(result[i].first != result[i - 1].first)
      mustResult.push_back(result[i]);

  std::cout << mustResult.size() << '\n';
  for (auto &edge : mustResult)
    std::cout << edge.first + 1 << ' ' << edge.second + 1 << '\n';
  std::cout << std::endl;

  return 0;
}
