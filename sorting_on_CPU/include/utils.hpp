#ifndef INCLUDE_UTILS_HPP
#define INCLUDE_UTILS_HPP

#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

namespace yLab {

template<typename T>
void dump(const std::vector<T>& vec) {
    for (const auto& value : vec) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

} // namespace yLab


#endif //INCLUDE_UTILS_HPP