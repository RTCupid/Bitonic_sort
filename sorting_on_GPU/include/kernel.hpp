#ifndef SORTING_ON_GPU_INCLUDE_KERNEL_HPP
#define SORTING_ON_GPU_INCLUDE_KERNEL_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "gpu_context.hpp"
#include "buffer.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>

namespace bLab {

class Kernel {
private:
    Gpu_context& context_;
    cl::Kernel kernel_;
    cl::Program program_;

public:
    Kernel(Gpu_context& context, const std::string& kernel_source,
           const std::string& kernel_name) : context_(context) {
        program_ = cl::Program(context_.get_context(), kernel_source);

        cl_int berr = program_.build({context.get_device()});
        if (berr != CL_SUCCESS) {
            std::string log =
                program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.get_device());
            throw std::runtime_error("OpenCL build failed (" +
                                     std::to_string(berr) + "):\n" + log);
        }

        kernel_ = cl::Kernel(program_, "bitonic_stage");
    }

    ~Kernel() = default;

    Kernel(const Kernel&) = delete;
    Kernel& operator=(const Kernel&) = delete;
    Kernel(Kernel&&) = default;
    Kernel& operator=(Kernel&&) = default;

    void set_arg(cl_uint index, const int& value) {
        cl_int err = kernel_.setArg(index, value);
        if (err != CL_SUCCESS) {
            throw std::runtime_error("Failed to set kernel argument: " +
                                   std::to_string(err));
        }
    }

    void set_arg(cl_uint index, const Buffer& buffer) {
        cl_int err = kernel_.setArg(index, buffer.get());
        if (err != CL_SUCCESS) {
            throw std::runtime_error("Failed to set kernel argument: " +
                                   std::to_string(err));
        }
    }

    cl::Kernel& get() { return kernel_; }
    const cl::Kernel& get() const { return kernel_; }
};

} // namespace bLab


#endif // SORTING_ON_GPU_INCLUDE_KERNEL_HPP
