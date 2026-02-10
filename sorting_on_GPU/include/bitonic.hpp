#ifndef SORTING_ON_GPU_INCLUDE_BITONIC_HPP
#define SORTING_ON_GPU_INCLUDE_BITONIC_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>

namespace bLab {

class Bitonic {
  private:
    std::vector<int> data_;

  public:
    Bitonic(std::vector<int> &data) : data_{data} {}

    static bool is_power_of_two(std::size_t x) {
      return x && ((x & (x - 1)) == 0);
    }

    static std::size_t next_power_of_two(std::size_t x) {
        std::size_t p = 1;
        while (p < x) p <<= 1;
        return p;
    }
    

    void sort() {
        cl::Platform platform = select_platform();

        // cl::string name = platform.getInfo<CL_PLATFORM_NAME>();
        // cl::string profile = platform.getInfo<CL_PLATFORM_PROFILE>();
        // std::cout << "Selected: " << name << ": " << profile << '\n';

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        if (devices.empty()) throw std::runtime_error("No GPU devices found");

        cl::Device device = devices[0];

        // std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>()
        //           << '\n';

        cl::Context context(device);
        cl::CommandQueue queue(context, device);

        const std::size_t n = data_.size();
        const std::size_t n2 = is_power_of_two(n) ? n : next_power_of_two(n);

        std::vector<int> padded = data_;
        padded.resize(n2, std::numeric_limits<int>::max());

        cl::Buffer buffer_on_gpu =
            move_buffer_to_gpu(context, queue, padded, platform);

        // const char *kernelSource = R"(
        //     __kernel void bitonic_sort(
        //       __global int *A, const unsigned int n) {
        //       for (int k = 2; k <= n; k *= 2) {
        //         for (int j = k/2; j >= 1; j /= 2) {
        //           for (int i = 0; i < k/2; ++i)
        //             A[i] = A[i+j];
        //         }
        //       }
        //     }
        // )";
        const std::string kernel_source = read_kernel("../sorting_on_GPU/include/kernel.cl");

        cl::Program program(context, kernel_source);

        cl_int berr = program.build({device});
        if (berr != CL_SUCCESS) {
            std::string log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
            throw std::runtime_error("OpenCL build failed (" + std::to_string(berr) + "):\n" + log);
        }

        cl::Kernel kernel(program, "bitonic_stage");

        cl::NDRange global(n2);

        for (cl_uint k = 2; k <= (cl_uint)n2; k <<= 1) {
            for (cl_uint j = k >> 1; j > 0; j >>= 1) {
                kernel.setArg(0, buffer_on_gpu);
                kernel.setArg(1, j);
                kernel.setArg(2, k);

                cl_int err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, cl::NullRange);
                if (err != CL_SUCCESS) {
                    throw std::runtime_error("enqueueNDRangeKernel failed: " + std::to_string(err));
                }
            }
        }

        queue.finish();

        queue.enqueueReadBuffer(buffer_on_gpu, CL_TRUE, 0, sizeof(int) * n2, padded.data());

        data_.assign(padded.begin(), padded.begin() + n);
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
