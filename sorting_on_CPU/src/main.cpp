#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "utils.hpp"

int main() {
    #ifdef ON_CPU_TIME
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<double, std::milli>;
    #endif

    std::size_t count;
    std::cin >> count;

    std::vector<int> data;
    int value;
    for (std::size_t i = 0; i < count; ++i) {
        while (std::cin >> value) {
            data.push_back(value);
        }
    }
    
    #ifndef ON_CPU_TIME
    std::cout << "data before sorting:" << std::endl;
    yLab::dump(data);
    #endif

    #ifdef ON_CPU_TIME
    auto start = clock::now();
    #endif

    std::sort(data.begin(), data.end());

    #ifdef ON_CPU_TIME
    auto end = clock::now();
    duration dt = end - start;
    #endif

    #ifndef ON_CPU_TIME
    std::cout << "data after sorting:" << std::endl;
    yLab::dump(data);
    #endif

    #ifdef ON_CPU_TIME
    std::cout << "Total time: " << dt.count() << " ms" << std::endl;
    #endif
}