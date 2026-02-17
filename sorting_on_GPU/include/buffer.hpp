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
    Gpu_context &context_;
    size_t size_;

  public:
    // @param context must live longer than Buffer
    Buffer(Gpu_context &context, const std::vector<int> &data,
           cl_mem_flags flags = CL_MEM_READ_WRITE)
        : context_(context), size_(data.size() * sizeof(int)) {
        buffer_ = cl::Buffer(context.get_context(), flags, size_);
        context_.get_queue().enqueueWriteBuffer(buffer_, CL_TRUE, 0, size_,
                                                data.data());
    }

    ~Buffer() = default;
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    Buffer(Buffer &&) = default;
    Buffer &operator=(Buffer &&) = delete;

    void read(std::vector<int> &data, bool blocking = true) {
        if (data.size() * sizeof(int) > size_) {
            throw std::runtime_error("Buffer too small for data");
        }
        context_.get_queue().enqueueReadBuffer(
            buffer_, blocking ? CL_TRUE : CL_FALSE, 0,
            data.size() * sizeof(int), data.data());
    }

    cl::Buffer &get() { return buffer_; }
    const cl::Buffer &get() const { return buffer_; }
    size_t size() const { return size_; }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_BUFFER_HPP
