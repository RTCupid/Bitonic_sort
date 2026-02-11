#ifndef SORTING_ON_GPU_INCLUDE_BUFFER_HPP
#define SORTING_ON_GPU_INCLUDE_BUFFER_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "gpu_context.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>

namespace bLab {

class Buffer {
private:
    cl::Buffer buffer_;
    Gpu_context& context_;
    size_t size_;

public:
    Buffer(Gpu_context& context, const std::vector<int>& data, cl_mem_flags flags = CL_MEM_READ_WRITE)
        : context_(context), size_(data.size() * sizeof(int)) {
//         cl_context_properties properties[] = {
//             CL_CONTEXT_PLATFORM,
//             reinterpret_cast<cl_context_properties>(context.get_platform()()),
//             0
//         };
//
//         cl::Context context_with_props(context.get_device(), properties);

        buffer_ = cl::Buffer(context.get_context(), flags, size_);

        cl::CommandQueue queue(context.get_context(), context.get_device());
        cl_int err = queue.enqueueWriteBuffer(buffer_, CL_TRUE, 0,
                                             size_, data.data());
        if (err != CL_SUCCESS) {
            throw std::runtime_error("Failed to copy data to GPU buffer: " +
                                   std::to_string(err));
        }
    }

    ~Buffer() = default;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    void read(std::vector<int>& data, bool blocking = true) {
        if (data.size() * sizeof(int) > size_) {
            throw std::runtime_error("Buffer too small for data");
        }
        cl_int err = context_.get_queue().enqueueReadBuffer(
            buffer_, blocking ? CL_TRUE : CL_FALSE, 0,
            data.size() * sizeof(int), data.data()
        );
        if (err != CL_SUCCESS) {
            throw std::runtime_error("Failed to read buffer: " +
                                   std::to_string(err));
        }
    }

    cl::Buffer& get() { return buffer_; }
    const cl::Buffer& get() const { return buffer_; }
    size_t size() const { return size_; }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_BUFFER_HPP
