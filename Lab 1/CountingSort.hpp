#ifndef COUNTING_SORT_HPP_
#define COUNTING_SORT_HPP_

#include <vector>
#include <memory>

struct Item {
    unsigned int key;
    unsigned long long value;
};

namespace sorts {

class CountingSort {
private:
    unsigned int minElement_;
    unsigned int maxElement_;

    void checkForMinMaxElements(std::vector<Item> & items) {
        minElement_ = items[0].key;
        maxElement_ = items[0].key;
        for (auto & item : items) {
            unsigned int currentKey = item.key;
            if (currentKey < minElement_) {
                minElement_ = currentKey;
            }

            if (currentKey > maxElement_) {
                maxElement_ = currentKey;
            }
        }
    }

public:
    CountingSort() = default;

    void sort(std::vector<Item> & items) {
        if (items.size() == 0) {
            return;
        }

        checkForMinMaxElements(items);
        std::vector<Item> tempItems(items);
        std::vector<unsigned int> elementsCounter(maxElement_ - minElement_ + 1);

        for (auto & item : items) {
            elementsCounter[item.key - minElement_]++;
        }

        for (int i = 1; i < elementsCounter.size(); ++i) {
            elementsCounter[i] += elementsCounter[i - 1];
        }


        for (int i = tempItems.size() - 1; i >= 0; --i) {
            long index = --elementsCounter[tempItems[i].key - minElement_];
            items[index] = tempItems[i];
        }
    }
};

} // !sorts

#endif // !COUNTING_SORT_HPP_