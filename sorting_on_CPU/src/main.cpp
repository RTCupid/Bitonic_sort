#include "utils.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    // #define TIME_TEST
    #ifdef TIME_TEST 
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<double, std::milli>;
    #endif

    std::size_t count;
    std::cin >> count;

    std::vector<int> data;
    int value;
    for (std::size_t i = 0; i < count && std::cin >> value; ++i)
        data.push_back(value);

    // std::cout << "data before sorting:" << std::endl;
    // bLab::dump(data);
    #ifdef TIME_TEST 
    auto start = clock::now();
    #endif
    std::sort(data.begin(), data.end());

    #ifdef TIME_TEST
    auto end = clock::now();
    duration dt = end - start;
    #endif

    // std::cout << "data after sorting:" << std::endl;
    #ifndef TIME_TEST
    bLab::dump(data);
    #endif
    #ifdef TIME_TEST 
    std::cout << "Total time: " << dt.count() << " ms" << std::endl;
    #endif
}