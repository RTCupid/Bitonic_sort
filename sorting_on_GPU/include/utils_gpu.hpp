#ifndef INCLUDE_UTILS_GPU_HPP
#define INCLUDE_UTILS_GPU_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include <CL/cl.h>
#include <CL/opencl.hpp>
#include <fstream>
#include <filesystem>
#include <sstream>

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

inline cl::Buffer move_buffer_to_gpu(cl::Context &context,
                                     cl::CommandQueue &queue,
                                     cl::vector<int> &buffer,
                                     cl::Platform &platform) {
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM,
        reinterpret_cast<cl_context_properties>(platform()),
        0 // signals end of property list
    };

    cl::Buffer buffer_on_gpu(context, CL_MEM_READ_WRITE,
                             buffer.size() * sizeof(int));

    queue.enqueueWriteBuffer(buffer_on_gpu, CL_TRUE, 0,
                             buffer.size() * sizeof(int), buffer.data());

    return buffer_on_gpu;
}

std::string read_kernel(const std::string &path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

inline std::filesystem::path executable_dir(const char* argv0) {
    std::filesystem::path exe = std::filesystem::absolute(argv0);

    exe = std::filesystem::canonical(exe);

    return exe.parent_path();
}

} // namespace bLab

#endif // INCLUDE_UTILS_HPP
