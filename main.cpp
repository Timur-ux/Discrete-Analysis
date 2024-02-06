#include <iostream>
#include "SearchingTree/AVLTree.hpp"
#include <cstdlib>

int main() {
    int n;
    std::cin >> n;
    tree::AvlTree<int, int> tree;
    for (int i = 1; i < n; ++i) {
        int n = rand();
        tree.insert(n, i);
        if (!tree.check_tree()) {
            std::cout << i << " <<< " << std::endl;
            break;
        }
    }
    tree.print(std::cout, 0);


    return 0;
}