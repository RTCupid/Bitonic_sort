#include "utils.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    // using clock = std::chrono::steady_clock;
    // using duration = std::chrono::duration<double, std::milli>;

    std::size_t count;
    std::cin >> count;

    std::vector<int> data;
    int value;
    for (std::size_t i = 0; i < count && std::cin >> value; ++i)
        data.push_back(value);

    // std::cout << "data before sorting:" << std::endl;
    // bLab::dump(data);

    // auto start = clock::now();

    std::sort(data.begin(), data.end());

    // auto end = clock::now();
    // duration dt = end - start;

    // std::cout << "data after sorting:" << std::endl;
    bLab::dump(data);

    // std::cout << "Total time: " << dt.count() << " ms" << std::endl;
}