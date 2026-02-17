#include "bitonic.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

int main(int argc, char **argv) {
    try {
#ifdef TIME_TEST
        using clock = std::chrono::steady_clock;
        using duration = std::chrono::duration<double, std::milli>;
#endif

        std::size_t count;
        if (!(std::cin >> count)) {
            std::cerr << "Error: Failed to read count\n";
            return EXIT_FAILURE;
        }

        std::vector<int> data;
        data.reserve(count);
        int value;
        for (std::size_t i = 0; i < count && std::cin >> value; ++i)
            data.push_back(value);

        if (data.empty()) {
            std::cerr << "Warning: No data to sort\n";
            return EXIT_SUCCESS;
        }

        auto exe_dir = bLab::executable_dir(argv[0]);
        auto kernel_path = exe_dir / "../" / "../" / "sorting_on_GPU" /
                           "include" / "kernel.cl";

        bLab::Bitonic bitonic{data, kernel_path.string()};

#ifdef TIME_TEST
        auto start = clock::now();
#endif
        bitonic.sort();

#ifdef TIME_TEST
        auto end = clock::now();
        duration dt = end - start;
#endif

#ifndef TIME_TEST
        bitonic.dump();
#endif
#ifdef TIME_TEST
        std::cout << "Total time: " << dt.count() << " ms" << std::endl;
#endif

        std::cout.flush();
        return EXIT_SUCCESS;

    } catch (const cl::Error &e) {
        try {
            std::cerr << "OpenCL Error: " << e.what() << " (code: " << e.err()
                      << ")\n";
        } catch (...) {
        }
        return EXIT_FAILURE;

    } catch (const std::bad_alloc &e) {
        try {
            std::cerr << "Memory Error: " << e.what() << "\n";
        } catch (...) {
        }
        return EXIT_FAILURE;

    } catch (const std::runtime_error &e) {
        try {
            std::cerr << "Runtime Error: " << e.what() << "\n";
        } catch (...) {
        }
        return EXIT_FAILURE;

    } catch (const std::exception &e) {
        try {
            std::cerr << "Standard Error: " << e.what() << "\n";
        } catch (...) {
        }
        return EXIT_FAILURE;

    } catch (...) {
        try {
            std::cerr << "Unknown Error: Unexpected exception occurred\n";
        } catch (...) {
        }
        return EXIT_FAILURE;
    }
}
