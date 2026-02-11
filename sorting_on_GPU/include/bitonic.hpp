#ifndef SORTING_ON_GPU_INCLUDE_BITONIC_HPP
#define SORTING_ON_GPU_INCLUDE_BITONIC_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "gpu_context.hpp"
#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

namespace bLab {

class Bitonic {
  private:
    std::vector<int> data_;
    const std::string kernel_path_;

  public:
    Bitonic(std::vector<int> &data, const std::string &kernel_path)
        : data_{data}, kernel_path_{kernel_path} {}

    void sort() {
        Gpu_context gpu_context{};

        const std::size_t n = data_.size();
        const std::size_t n2 = is_power_of_two(n) ? n : next_power_of_two(n);

        std::vector<int> padded = data_;
        padded.resize(n2, std::numeric_limits<int>::max());

        cl::Buffer buffer_on_gpu =
            move_buffer_to_gpu(gpu_context.context, gpu_context.queue, padded,
                               gpu_context.platform);

        const std::string kernel_source = read_kernel(kernel_path_);

        cl::Program program(gpu_context.context, kernel_source);

        cl_int berr = program.build({gpu_context.device});
        if (berr != CL_SUCCESS) {
            std::string log =
                program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu_context.device);
            throw std::runtime_error("OpenCL build failed (" +
                                     std::to_string(berr) + "):\n" + log);
        }

        cl::Kernel kernel(program, "bitonic_stage");

        cl::NDRange global(n2);

        for (cl_uint k = 2; k <= (cl_uint)n2; k <<= 1) {
            for (cl_uint j = k >> 1; j > 0; j >>= 1) {
                kernel.setArg(0, buffer_on_gpu);
                kernel.setArg(1, j);
                kernel.setArg(2, k);

                cl_int err = gpu_context.queue.enqueueNDRangeKernel(
                    kernel, cl::NullRange, global, cl::NullRange);
                if (err != CL_SUCCESS) {
                    throw std::runtime_error("enqueueNDRangeKernel failed: " +
                                             std::to_string(err));
                }
            }
        }

        gpu_context.queue.finish();

        gpu_context.queue.enqueueReadBuffer(buffer_on_gpu, CL_TRUE, 0,
                                            sizeof(int) * n2, padded.data());

        data_.assign(padded.begin(), padded.begin() + n);
    }

    void dump() const {
        for (const auto &value : data_) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

  private:
    static bool is_power_of_two(std::size_t x) {
        return x && ((x & (x - 1)) == 0);
    }

    static std::size_t next_power_of_two(std::size_t x) {
        std::size_t p = 1;
        while (p < x)
            p <<= 1;
        return p;
    }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_BITONIC_HPP
