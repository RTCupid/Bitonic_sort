#ifndef INCLUDE_UTILS_GPU_HPP
#define INCLUDE_UTILS_GPU_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include <CL/cl.h>
#include <CL/opencl.hpp>

namespace bLab {

inline cl::Platform select_platform() {
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

inline cl::Buffer move_buffer_to_gpu(cl::Context &context, cl::CommandQueue &queue, cl::vector<int> &buffer,
                               cl::Platform &platform) {
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM,
        reinterpret_cast<cl_context_properties>(platform()),
        0 // signals end of property list
    };

    cl::Buffer buffer_on_gpu(context, CL_MEM_READ_WRITE, buffer.size() * sizeof(int));

    queue.enqueueWriteBuffer(buffer_on_gpu, CL_TRUE, 0,
                                buffer.size() * sizeof(int),
                                buffer.data());

    return buffer_on_gpu;
}

} // namespace bLab

#endif // INCLUDE_UTILS_HPP
