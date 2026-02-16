#ifndef SORTING_ON_GPU_INCLUDE_CONTEXT_HPP
#define SORTING_ON_GPU_INCLUDE_CONTEXT_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>

namespace bLab {

class Gpu_context {
  private:
    cl::Platform platform_;
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;

  public:
    Gpu_context() {
        auto [platform, type] = select_platform();

        std::vector<cl::Device> devices;
        if (type == type_device::gpu)
            platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        else
            platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);

        if (devices.empty())
            throw std::runtime_error("No devices found");

        device_ = devices[0];

        context_ = cl::Context(device_);
        queue_ = cl::CommandQueue(context_, device_);
    }

    ~Gpu_context() = default;

    Gpu_context(const Gpu_context &) = delete;
    Gpu_context &operator=(const Gpu_context &) = delete;
    Gpu_context(Gpu_context &&) = default;
    Gpu_context &operator=(Gpu_context &&) = default;

    cl::CommandQueue &get_queue() { return queue_; }
    const cl::CommandQueue &get_queue() const { return queue_; }
    cl::Context &get_context() { return context_; }
    const cl::Context &get_context() const { return context_; }
    cl::Device &get_device() { return device_; }
    const cl::Device &get_device() const { return device_; }
    cl::Platform &get_platform() { return platform_; }
    const cl::Platform &get_platform() const { return platform_; }

    void finish() { queue_.finish(); }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_CONTEXT_HPP
