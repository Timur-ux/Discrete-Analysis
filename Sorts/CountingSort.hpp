#ifndef COUNTING_SORT_HPP_
#define COUNTING_SORT_HPP_

#include <vector>
#include <concepts>

template <typename T>
concept IsInteger = std::same_as<T, int> || std::same_as<T, long> || std::same_as<T, long long>;

namespace sorts {

template <IsInteger TElement>
class CountingSort {
private:
    TElement minElement_;
    TElement maxElement_;

    void checkForMinMaxElements(std::vector<TElement> & values) {
        minElement_ = values[0];
        maxElement_ = values[0];
        for (TElement & value : values) {
            if (value < minElement_) {
                minElement_ = value;
            }

            if (value > maxElement_) {
                maxElement_ = value;
            }
        }
    }

public:
    CountingSort() = default;

    void sort(std::vector<TElement> & values) {
        if (values.size() == 0) {
            return;
        }

        checkForMinMaxElements(values);
        std::vector<TElement> elementsCounter(maxElement_ - minElement_ + 1);

        for (TElement & value : values) {
            elementsCounter[value - minElement_]++;
        }

        TElement lastInserted = 0;
        for (int num = 0; num < elementsCounter.size(); ++num) {
            if (elementsCounter[num] == 0) {
                continue;
            }

            for (int i = 0; i < elementsCounter[num]; ++i) {
                values[lastInserted++] = num + minElement_;
            }
        }
    }
};

} // !sorts

#endif // !COUNTING_SORT_HPP_