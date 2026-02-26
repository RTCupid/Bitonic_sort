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
#ifdef TIME_TEST
    double total_time{0};
    double last_kernel_ms_{0};
    double last_h2d_ms_{0};
    double last_d2h_ms_{0};
#endif

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
#ifdef TIME_TEST
        gpu_context_.finish();
        last_h2d_ms_ = event_ms(buffer.get_last_write_event());
#endif

        Kernel kernel_local(gpu_context_, kernel_source_, "bitonic_sort_local");
        Kernel kernel_global(gpu_context_, kernel_source_,
                             "bitonic_sort_global");

        try {
            run_bitonic_sort(kernel_local, kernel_global, buffer, n);
            buffer.read(padded, true);

#ifdef TIME_TEST
            gpu_context_.finish();
            last_d2h_ms_ = event_ms(buffer.get_last_read_event());
            total_time = last_h2d_ms_ + last_kernel_ms_ + last_d2h_ms_;
#endif

            data_.assign(padded.begin(), padded.begin() + data_.size());
        } catch (const cl::Error &e) {
            valid_ = false;
            throw;
        }
    }

    void dump() const {
        bool first = true;
        for (const auto &value : data_) {
            if (!first) {
                std::cout << " ";
            }
            std::cout << value;
            first = false;
        }
        std::cout << std::endl;
    }

#ifdef TIME_TEST
    double cl_time_ms() const noexcept { return total_time; }
    double cl_h2d_ms() const noexcept { return last_h2d_ms_; }
    double cl_kernel_ms() const noexcept { return last_kernel_ms_; }
    double cl_d2h_ms() const noexcept { return last_d2h_ms_; }
#endif

  private:
    void run_bitonic_sort(Kernel &kernel_local, Kernel &kernel_global,
                          Buffer &buffer, const size_t &n) {
        const size_t local_size = 256;

        size_t num_blocks = (n + local_size - 1) / local_size;

        cl::NDRange global(num_blocks * local_size);
        cl::NDRange local(local_size);

        auto &queue = gpu_context_.get_queue();

        std::vector<cl::Event> events;
        // ==============================================
        // PHASE 1: Sorting within blocks (Local Memory)
        // ================================================
        // We run the kernel for k = 2, 4, 8, ..., LOCAL_SIZE.
        // The kernel will loop over j.

        for (cl_uint k = 2; k <= local_size; k <<= 1) {
            kernel.set_arg(0, buffer);
            kernel.set_arg(1, k);
            kernel.set_arg(2, (cl_uint)0); // j = 0 (ignored in local mode)
            kernel.set_arg(3, (cl_uint)n);
            kernel.set_arg(4, (cl_uint)1); // use_local_memory = true
            kernel.set_arg(5, (cl_uint)local_size);

            ON_TIME_TEST(cl::Event event;)
            queue.enqueueNDRangeKernel(kernel.get(), cl::NullRange, global,
                                       local ON_TIME_TEST(, nullptr, &event));
            ON_TIME_TEST(events.push_back(event);)
        }

        // ===============================================
        // PHASE 2: Block Merging (Global Memory)
        // ================================================

        for (cl_uint k = local_size * 2; k <= (cl_uint)n; k <<= 1) {
            for (cl_uint j = k >> 1; j > 0; j >>= 1) {
                kernel.set_arg(0, buffer);
                kernel.set_arg(1, k);
                kernel.set_arg(2, j);
                kernel.set_arg(3, (cl_uint)n);
                kernel.set_arg(4, (cl_uint)0); // use_local_memory = false
                kernel.set_arg(5, (cl_uint)local_size);

                ON_TIME_TEST(cl::Event event;)
                queue.enqueueNDRangeKernel(
                    kernel.get(), cl::NullRange, global,
                    local ON_TIME_TEST(, nullptr, &event));
                ON_TIME_TEST(events.push_back(event);)
            }
        }
#ifdef TIME_TEST
        gpu_context_.finish();
        last_kernel_ms_ = 0.0;
        for (const auto &ev : events)
            last_kernel_ms_ += event_ms(ev);
#endif
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
