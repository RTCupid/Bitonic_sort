#include "bitonic.hpp"
#include <chrono>
#include <iostream>
#include <ostream>
#include <vector>

int main(int argc, char **argv) {
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

    auto exe_dir = bLab::executable_dir(argv[0]);
    auto kernel_path =
        exe_dir / "../" / "../" / "sorting_on_GPU" / "include" / "kernel.cl";

    bLab::Bitonic bitonic{data, kernel_path.string()};

    // std::cout << "data before sorting:" << std::endl;
    // bitonic.dump();

#ifdef TIME_TEST
    auto start = clock::now();
#endif
    bitonic.sort();

#ifdef TIME_TEST
    auto end = clock::now();
    duration dt = end - start;
#endif

// std::cout << "data after sorting:" << std::endl;
#ifndef TIME_TEST
    bitonic.dump();
#endif
#ifdef TIME_TEST
    std::cout << "Total time: " << dt.count() << " ms" << std::endl;
#endif
}
