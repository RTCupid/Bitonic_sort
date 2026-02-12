#ifndef SORTING_ON_GPU_INCLUDE_BITONIC_HPP
#define SORTING_ON_GPU_INCLUDE_BITONIC_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "buffer.hpp"
#include "gpu_context.hpp"
#include "kernel.hpp"
#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <iostream>
#include <limits>
#include <vector>

namespace bLab {

static bool is_power_of_two(std::size_t x) { return x && ((x & (x - 1)) == 0); }

static std::size_t next_power_of_two(std::size_t x) {
    std::size_t p = 1;
    while (p < x)
        p <<= 1;
    return p;
}

class Bitonic {
  private:
    const std::string kernel_source_;
    std::vector<int> data_;
    Gpu_context gpu_context_;
    bool valid_ = true;

  public:
    Bitonic(std::vector<int> &data, const std::string &kernel_path)
        : kernel_source_{read_kernel(kernel_path)}, data_{data} {}

    void sort() {
        if (data_.empty())
            return;
        if (!valid_)
            throw std::runtime_error("Invalid state");

        auto padded = pad_data_to_power_of_two();
        
        auto n = padded.size();

        Buffer buffer(gpu_context_, padded);

        Kernel kernel(gpu_context_, kernel_source_, "bitonic_stage");

        try {
            run_bitonic_sort(gpu_context_, kernel, buffer, n);
            gpu_context_.finish();
            buffer.read(padded, true);

            data_.assign(padded.begin(), padded.begin() + data_.size());
        } catch (const cl::Error &e) {
            valid_ = false;
            std::cerr << "OpenCL error: " << e.what() << '\n';
            throw;
        }
    }

    void dump() const {
        for (const auto &value : data_) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

  private:
    void run_bitonic_sort(Gpu_context &gpu_context, Kernel &kernel,
                          Buffer &buffer, const size_t &n) {
        cl::NDRange global(n);

        for (cl_uint k = 2; k <= (cl_uint)n; k <<= 1) {
            for (cl_uint j = k >> 1; j > 0; j >>= 1) {
                kernel.set_arg(0, buffer);
                kernel.set_arg(1, j);
                kernel.set_arg(2, k);

                gpu_context.get_queue().enqueueNDRangeKernel(
                    kernel.get(), cl::NullRange, global, cl::NullRange);
            }
        }
    }

    std::vector<int> pad_data_to_power_of_two() const {
        const std::size_t n = data_.size();
        const std::size_t n2 = is_power_of_two(n) ? n : next_power_of_two(n);

        std::vector<int> padded = data_;
        padded.resize(n2, std::numeric_limits<int>::max());

        return padded;
    }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_BITONIC_HPP
