#ifndef INCLUDE_GPU_CONTEXT_HPP
#define INCLUDE_GPU_CONTEXT_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>

namespace bLab {

class Gpu_context {
  public:
    cl::Platform platform;
    cl::Device device;
    cl::Context context;
    cl::CommandQueue queue;

    Gpu_context() {
        auto [platform, type] = select_platform();

        std::vector<cl::Device> devices;
        if (type == type_device::gpu)
            platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        else
            platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);

        if (devices.empty())
            throw std::runtime_error("No devices found");

        device = devices[0];

        context = cl::Context(device);
        queue = cl::CommandQueue(context, device);
    }
};

} // namespace bLab

#endif // INCLUDE_GPU_CONTEXT_HPP
