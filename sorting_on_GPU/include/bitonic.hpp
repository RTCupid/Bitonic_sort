#ifndef SORTING_ON_GPU_INCLUDE_BITONIC_HPP
#define SORTING_ON_GPU_INCLUDE_BITONIC_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>
#include "utils_gpu.hpp"

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
        std::cout << "Selected: " << name << ": " << profile << '\n';

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

        cl::Device device = devices[0];

        std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << '\n';

        cl::Context context(device);
        cl::CommandQueue queue(context, device);

        auto buffer_on_gpu = move_buffer_to_gpu(context, queue, data_, platform);

        const char* kernelSource = R"(
            __kernel void bitonic_sort(
              __global int *A, const unsigned int n) {
              for (int k = 2; k <= n; k *= 2) {
                for (int j = k/2; j >= 1; j /= 2) {
                  for (int i = 0; i < k/2; ++i)
                    A[i] = A[i+j];
                }
              }
            }
        )";

        cl::Program program(context, kernelSource);

        program.build({device});

        cl::Kernel kernel(program, "bitonic_sort");

        kernel.setArg(0, buffer_on_gpu);
        kernel.setArg(1, data_.size());

        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(data_.size()), cl::NullRange);

        queue.enqueueReadBuffer(buffer_on_gpu, CL_TRUE, 0,
                               sizeof(float) * data_.size(), data_.data());

        queue.finish();
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
