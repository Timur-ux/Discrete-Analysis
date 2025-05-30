//
//  kihm_algorithm.cpp
//  Kuhn_algorithm
//
//  Created by Nihad on 11/17/2019.
//  Copyright © 2019 Nihad.Corp. All rights reserved.
//

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <random>
#include <vector>

using namespace std;
typedef std::chrono::high_resolution_clock Clock;
vector<vector<int>> bipartiteGraph;
vector<int> match;
int n1, n2;
vector<char> usedVertices;
using Graph = vector<vector<int>>;

template <typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &bipartiteGraph) {
  std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
  std::advance(start, dis(bipartiteGraph));
  return start;
}

template <typename Iter> Iter select_randomly(Iter start, Iter end) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return select_randomly(start, end, gen);
}

void print2DArray(vector<vector<int>> array2D) {
  int l = 1;
  for (auto i = array2D.begin(); i != array2D.end(); ++i) {
    cout << l++ << " - {";
    for (auto j = i->begin(); j != i->end(); ++j) {
      cout << *j + 1;
      if (j == (i->end() - 1))
        continue;
      cout << ", ";
    }
    cout << "}" << endl;
  }
  cout << endl;
}

bool kuhnAlgorithm(int x) {
  if (usedVertices[x])
    return false;
  usedVertices[x] = true;
  for (size_t i = 0; i < bipartiteGraph[x].size(); ++i) {
    int to = bipartiteGraph[x][i];
    if (match[to] == -1 || kuhnAlgorithm(match[to])) {
      match[to] = x;
      return true;
    }
  }
  return false;
}

vector<vector<int>> generateBipartiteGraph(int n1, int n2) {
  auto resultBipartiteGraph = vector<vector<int>>(n1, vector<int>(n2));
  for (int i = 0; i < n1; ++i) {
    vector<int> row;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist6(0, n2 - 1);
    int numberOfEdges = dist6(rng);
    auto secondSetMatchedVertices = vector<int>(n2);
    iota(secondSetMatchedVertices.begin(), secondSetMatchedVertices.end(), 0);
    for (int z = 0; z < numberOfEdges; ++z) {
      int randElement = *select_randomly(secondSetMatchedVertices.begin(),
                                         secondSetMatchedVertices.end());
      row.push_back(randElement);
      secondSetMatchedVertices.erase(remove(secondSetMatchedVertices.begin(),
                                            secondSetMatchedVertices.end(),
                                            randElement),
                                     secondSetMatchedVertices.end());
    }
    resultBipartiteGraph[i] = row;
  }
  return resultBipartiteGraph;
}

void checkOnTimer(int n1, int n2, int step, int iterationsNumber) {
  for (int i = 0; i < iterationsNumber; ++i) {
    auto timeValues = vector<double>();
    for (int j = 0; j < 10; ++j) {
      bipartiteGraph = generateBipartiteGraph(n1, n2);
      auto t1 = Clock::now();
      // Kuhn algorithm
      match.assign(n2, -1);
      for (int x = 0; x < n1; ++x) {
        usedVertices.assign(n1, false);
        kuhnAlgorithm(x);
      }
      auto t2 = Clock::now();
      timeValues.push_back(
          (double)chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1)
              .count());
    }
    double avrgComputedTime =
        accumulate(timeValues.begin(), timeValues.end(), 0) / 10.0;
    double inMiliseconds = avrgComputedTime / 1000000.0;
    cout << n1 << " - " << fixed << inMiliseconds << endl;
    n1 += step;
    n2 = n1;
  }
}

enum class Side { none = -1, left, right };

std::vector<Side> divide(Graph &graph) {
  std::vector<bool> used(graph.size(), false);
  std::vector<Side> result(graph.size(), Side::none);
  std::queue<int> vertexes;
  vertexes.push(0);
  result[0] = Side::left;
  used[0] = true;

  while (!vertexes.empty()) {
    int v = vertexes.front();
    vertexes.pop();

    Side opposite = result[v] == Side::left ? Side::right : Side::left;
    for (auto &to : graph[v]) {
      if (!used[to]) {
        used[to] = true;
        result[to] = opposite;
        vertexes.push(to);
      }
    }
  }

  return result;
}

int main() {
  long long int n, m;

  std::cin >> n >> m;
  Graph g(n);
  long long int u, v;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> u >> v;
    g[u - 1].push_back(v - 1);
    g[v - 1].push_back(u - 1);
  }

  auto sides = divide(g);
  std::vector<int> leftSide;
  for(size_t i = 0; i < sides.size(); ++i) {
    if(sides[i] == Side::left) {
      leftSide.push_back(i);
    }
  }


  
  n1 = 5;  // Number of vertices of the FIRST set of a bipartite graph
  n2 = n1; // // Number of vertices of the SECOND set of a bipartite graph
  bipartiteGraph = generateBipartiteGraph(
      n1, n2); // Generating "random" bipartite graph with connected edges

  // Kuhn algorithm
  match.assign(n2, -1);
  for (int x = 0; x < n1; ++x) {
    usedVertices.assign(n1, false);
    kuhnAlgorithm(x);
  }

  print2DArray(bipartiteGraph); // Printing the inital bipartite graph

  // Printing the resulting maximum matching
  cout << "Maximum matching: " << endl;
  for (int i = 0; i < n2; ++i)
    if (match[i] != -1)
      cout << match[i] + 1 << " - " << i + 1 << endl;
  cout << endl;

  // Checking the algorithm "speed"
  cout << "Starting time complexity evaluations of algorithm for 'randomly' "
          "generated bipartite graphs"
       << endl;
  n1 = 100;
  n2 = n1;
  int step = 100;
  int iterationsNumber = 15;
  checkOnTimer(n1, n2, step, iterationsNumber);

  return 0;
}
