#ifndef SORTING_ON_GPU_INCLUDE_KERNEL_HPP
#define SORTING_ON_GPU_INCLUDE_KERNEL_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "buffer.hpp"
#include "gpu_context.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>

namespace bLab {

class Kernel {
  private:
    Gpu_context &context_;
    cl::Kernel kernel_;
    cl::Program program_;

  public:
    // @param context must live longer than Kernel
    Kernel(Gpu_context &context, const std::string &kernel_source,
           const std::string &kernel_name)
        : context_(context) {
        program_ = cl::Program(context_.get_context(), kernel_source);
        program_.build({context.get_device()});
        kernel_ = cl::Kernel(program_, kernel_name.c_str());
    }

    ~Kernel() = default;

    Kernel(const Kernel &) = delete;
    Kernel &operator=(const Kernel &) = delete;
    Kernel(Kernel &&) = default;
    Kernel &operator=(Kernel &&) = delete;

    void set_arg(cl_uint index, const int &value) {
        kernel_.setArg(index, value);
    }

    void set_arg(cl_uint index, const Buffer &buffer) {
        kernel_.setArg(index, buffer.get());
    }

    cl::Kernel &get() { return kernel_; }
    const cl::Kernel &get() const { return kernel_; }
};

} // namespace bLab

#endif // SORTING_ON_GPU_INCLUDE_KERNEL_HPP
