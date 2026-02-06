#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include <CL/cl.h>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <CL/opencl.hpp>
#include "bitonic.hpp"

using namespace bLab;

namespace {

cl::Platform select_platform() {
    cl::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    for (auto p : platforms) {
        cl_uint num_devices = 0;
        clGetDeviceIDs(p(), CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        if (num_devices > 0)
            return cl::Platform(p);
    }
    throw std::runtime_error("No platform selected");
}

enum { BUFSZ = 128 };

} // namespace

int main() {
    cl::Platform P = select_platform();
    cl::string name = P.getInfo<CL_PLATFORM_NAME>();
    cl::string profile = P.getInfo<CL_PLATFORM_PROFILE>();
    std::cout << "Selected: " << name << ": " << profile << std::endl;

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(P()),
        0 // signals end of property list
    };

    cl::Context C(CL_DEVICE_TYPE_GPU, properties);
    cl::CommandQueue Q(C);

    cl::vector<int> A(BUFSZ), B(BUFSZ);
    for (int i = 0; i < BUFSZ; ++i)
        A[i] = i * i;

    cl::Buffer D(C, CL_MEM_READ_WRITE, BUFSZ * sizeof(int));

    cl::copy(Q, A.begin(), A.end(), D);
    cl::copy(Q, D, B.begin(), B.end());

    for (int i = 0; i < BUFSZ; ++i) {
        if (B[i] != i * i) {
            std::cout << "Error at: " << i << ": " << B[i] << " != " << i * i << std::endl;
            return -1;
        }
    }

    std::cout << "Checks passed" << std::endl;
}

//     using clock = std::chrono::steady_clock;
//     using duration = std::chrono::duration<double, std::milli>;
//
//     std::size_t count;
//     std::cin >> count;
//
//     std::vector<int> data;
//     int value;
//     for (std::size_t i = 0; i < count && std::cin >> value; ++i)
//         data.push_back(value);
//
//     Bitonic bitonic{data};
//
//     std::cout << "data before sorting:" << std::endl;
//     bitonic.dump();
//
//     auto start = clock::now();
//     bitonic.bitonic_sort();
//     auto end = clock::now();
//     duration dt = end - start;
//
//     std::cout << "data after sorting:" << std::endl;
//     bitonic.dump();
//
//     std::cout << "Total time: " << dt.count() << " ms" << std::endl;
//}
