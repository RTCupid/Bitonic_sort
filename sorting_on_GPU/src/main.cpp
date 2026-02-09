#include "bitonic.hpp"
#include <chrono>
#include <iostream>
#include <ostream>
#include <vector>

int main() {
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<double, std::milli>;

    std::size_t count;
    std::cin >> count;

    std::vector<int> data;
    int value;
    for (std::size_t i = 0; i < count && std::cin >> value; ++i)
        data.push_back(value);

    bLab::Bitonic bitonic{data};

    std::cout << "data before sorting:" << std::endl;
    bitonic.dump();

    auto start = clock::now();
    bitonic.sort();
    auto end = clock::now();
    duration dt = end - start;

    std::cout << "data after sorting:" << std::endl;
    bitonic.dump();

    std::cout << "Total time: " << dt.count() << " ms" << std::endl;
}
