#ifndef SORTING_ON_GPU_INCLUDE_BITONIC_HPP
#define SORTING_ON_GPU_INCLUDE_BITONIC_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>

namespace bLab {

class Bitonic {
  private:
    std::vector<int> data_;

    void merge() {}

    void split() {}

  public:
    Bitonic(std::vector<int> &data) : data_{data} {}

    void sort() {
        cl::Platform platform = select_platform();
        cl::string name = platform.getInfo<CL_PLATFORM_NAME>();
        cl::string profile = platform.getInfo<CL_PLATFORM_PROFILE>();
        std::cout << "Selected: " << name << ": " << profile << std::endl;

        move_buffer_to_gpu(data_, platform);
    }

    void dump() const {
        for (const auto &value : data_) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_BITONIC_HPP
