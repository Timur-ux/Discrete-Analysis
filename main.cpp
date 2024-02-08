#include <iostream>
#include "SearchingTree/AVLTree.hpp"
#include <cstdlib>
#include <vector>
#include <fstream>

int main() {
    int n;
    std::cin >> n;

    std::ofstream log("log.txt");
    log << "----------INSERTING----------" << std::endl;

    tree::AvlTree<int, int> tree;
    std::vector<int> keys;
    for (int i = 1; i < n; ++i) {
        int n = rand();
        keys.emplace_back(n);

        log << "Step: " << i << " key: " << n << std::endl;

        tree.insert(n, i);

        // tree.print(log);
        if (!tree.check_tree()) {
            std::cout << i << " <<< " << std::endl;
            break;
        }
        log << "---------------" << std::endl;
    }
    log << "----------REMOVING----------" << std::endl;
    for (int i = 1; i < n; ++i) {
        int index = rand() % keys.size();
        int key = keys[index];
        keys.erase(keys.begin() + index);

        log << "Removing: " << key << std::endl;
        tree.remove(key);
        // tree.print(log);
        if (!tree.check_tree()) {
            std::cout << i << " <<< " << std::endl;
            break;
        }
        log << "---------------" << std::endl;
    }
    // tree.print(std::cout, 0);


    return 0;
}