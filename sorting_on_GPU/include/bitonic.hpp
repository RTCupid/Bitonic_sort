#ifndef SORTING_ON_GPU_INCLUDE_BITONIC_HPP
#define SORTING_ON_GPU_INCLUDE_BITONIC_HPP

#include <vector>
#include <iostream>

namespace bitonic {

class Bitonic {
private:
    std::vector<int> data_;

    void bitonic_merge() {}

    void bitonic_split() {}
public:
    Bitonic(std::vector<int> &data) : data_{data} {}

    void bitonic_sort() {}

    void dump() const {
        for (const auto& value : data_) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

} // namespace bitonic

#endif // SORTING_ON_GPU_INCLUDE_BITONIC_HPP
