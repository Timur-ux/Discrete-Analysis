#ifndef BUCKET_SORT_HPP_
#define BUCKET_SORT_HPP_

#include <type_traits>
#include <concepts>
#include <vector>
#include <stdexcept>
#include <algorithm>

#define all(x) std::begin(x), std::end(x)

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

namespace sorts {

namespace impl {
template <FloatingPoint TVal>
struct MinMaxStorage;
} // ! impl

template <FloatingPoint TVal>
class BucketSort {
private:
    size_t segmentationSize_ = 10;
    impl::MinMaxStorage<TVal> impl_checkForMinMax(std::vector<TVal> & values);
public:
    void sort(std::vector<TVal> & values);
    void setSegmentationSize(size_t segmentationSize);
};

namespace impl {
template <FloatingPoint TVal>
struct MinMaxStorage {
    TVal min;
    TVal max;
};
} // !impl

template<FloatingPoint TVal>
inline impl::MinMaxStorage<TVal>
BucketSort<TVal>::impl_checkForMinMax(std::vector<TVal> & values) {
    TVal min = values[0];
    TVal max = min;

    for (TVal & value : values) {
        if (value < min) {
            min = value;
        }

        if (value > max) {
            max = value;
        }
    }

    return impl::MinMaxStorage{ .min = min, .max = max };
}

template<FloatingPoint TVal>
inline void
BucketSort<TVal>::sort(std::vector<TVal> & values) {
    if (values.size() == 0) {
        return;
    }

    auto [min, max] = impl_checkForMinMax(values);
    TVal delta = max - min;
    TVal segmentSize = delta / (segmentationSize_ - 1);
    std::vector<std::vector<TVal>> segmentatedValues(segmentationSize_);

    for (TVal & value : values) {
        size_t index = (value - min) / segmentSize;
        segmentatedValues[index].emplace_back(value);
    }

    long long lastInserted = 0;
    for (std::vector<TVal> & currentSegmentValues : segmentatedValues) {
        std::stable_sort(all(currentSegmentValues));

        for (TVal & value : currentSegmentValues) {
            values[lastInserted++] = value;
        }
    }
}

template<FloatingPoint TVal>
inline void
BucketSort<TVal>::setSegmentationSize(size_t segmentationSize) {
    segmentationSize_ = segmentationSize;
}

} // !sorts

#endif // !BUCKET_SORT_HPP_