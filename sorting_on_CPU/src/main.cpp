#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "utils.hpp"


int main() {
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<double, std::milli>;

    std::vector<int> data = {5, 2, 9, 1, 5, 6, 9, 3, 6, 2};
    
    std::cout << "data before sorting:" << std::endl;
    yLab::dump(data);

    auto start = clock::now();
    std::sort(data.begin(), data.end());
    auto end = clock::now();
    duration dt = end - start;

    std::cout << "data after sorting:" << std::endl;
    yLab::dump(data);

    std::cout << "Total time: " << dt.count() << " ms" << std::endl;
}