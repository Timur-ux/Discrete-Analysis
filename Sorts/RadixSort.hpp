#ifndef RADIX_SORT_HPP_
#define RADIX_SORT_HPP_

#include <concepts>
#include <vector>
#include <stdexcept>
#include <list>

namespace sorts {

template <typename T>
concept IsInteger = std::same_as<T, int> || std::same_as<T, long> || std::same_as<T, long long>;

struct MinMaxStorage {
    long long minMask;
    long long maxMask;
    long long minAbs;
};

template <IsInteger TVal>
class RadixSort {
private:
    int r_ = 1;
    long long bitMask_ = 1;

public:

    void sort(std::vector<TVal> & values);

    void setSortingStep(int r);

private:

    class Impl_countingBitMaskSort {
    private:
        int offset_ = 0;
        long long bitMask_ = 1;
        MinMaxStorage checkForMinMax(std::vector<TVal> & values);
    public:
        void sort(std::vector<TVal> & values);
        void setOffset(int offset);
        void setBitMask(long long bitMask);
    };

    Impl_countingBitMaskSort sorter_;
};



template<IsInteger TVal>
inline void
RadixSort<TVal>::sort(std::vector<TVal> & values) {
    sorter_.setBitMask(bitMask_);
    for (int offset = 0; offset < (sizeof(TVal) * 8); offset += r_) {
        sorter_.setOffset(offset);
        sorter_.sort(values);
    }
}

template<IsInteger TVal>
inline void RadixSort<TVal>::setSortingStep(int r) {
    if (r <= 0) {
        throw std::invalid_argument("Sorting step must be positive value");
    }
    if (r > sizeof(TVal) * 8) {
        throw std::invalid_argument("Sorting step must be less than "
        + std::to_string(sizeof(TVal) * 8 + 1));
    }

    r_ = r;
    long long bitMask = 1;
    for (int _ = 0; _ < r_; _++) {
        bitMask *= 2;
    }

    bitMask_ = bitMask - 1;
}

template <IsInteger TVal>
inline MinMaxStorage
RadixSort<TVal>::Impl_countingBitMaskSort::checkForMinMax(std::vector<TVal> & values) {
    long long minMask = (values[0] >> offset_) & bitMask_;
    long long maxMask = minMask;
    long long minAbs = values[0];

    for (TVal & value : values) {
        if (value < minAbs) {
            minAbs = value;
        }
    }

    for (TVal & value : values) {
        long long current = ((value - minAbs) >> offset_) & bitMask_;
        if (current > maxMask) {
            maxMask = current;
        }

        if (current < minMask) {
            minMask = current;
        }
    }

    return MinMaxStorage{ minMask, maxMask, minAbs };
}

template<IsInteger TVal>
inline void
RadixSort<TVal>::Impl_countingBitMaskSort::sort(std::vector<TVal> & values) {
    if (values.size() == 0) {
        return;
    }

    auto [minMask, maxMask, minAbs] = checkForMinMax(values);
    std::cout << "MinMask: " << minMask << " MaxMask: " << maxMask << std::endl;
    std::vector<std::list<TVal>> elementsCounter(maxMask - minMask + 1);

    for (TVal & value : values) {
        long long index = ((value - minAbs) >> offset_) & bitMask_;
        elementsCounter[index].push_back(value);
    }

    long long lastInserted = 0;
    for (int i = 0; i < elementsCounter.size(); ++i) {
        if (elementsCounter[i].size() == 0) {
            continue;
        }
        for (TVal & value : elementsCounter[i]) {
            values[lastInserted++] = value;
        }
    }
}

template<IsInteger TVal>
inline void
RadixSort<TVal>::Impl_countingBitMaskSort::setOffset(int offset) {
    if (offset < 0) {
        throw std::invalid_argument("Offset must be not negative value");
    }

    offset_ = offset;
}

template<IsInteger TVal>
inline void
RadixSort<TVal>::Impl_countingBitMaskSort::setBitMask(long long bitMask) {
    long long temp = bitMask + 1;
    while (temp > 1) {
        if (temp % 2 != 0) {
            throw std::invalid_argument("Bit mask must be 000...00111...111 type");
        }
        temp /= 2;
    }

    bitMask_ = bitMask;
}


} // !sorts

#endif // !RADIX_SORT_HPP_