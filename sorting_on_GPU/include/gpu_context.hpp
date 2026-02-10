#ifndef INCLUDE_GPU_CONTEXT_HPP
#define INCLUDE_GPU_CONTEXT_HPP

#define CL_TARGET_OPENCL_VERSION 300
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include "utils_gpu.hpp"
#include <CL/cl.h>
#include <CL/opencl.hpp>

class Gpu_context { // TODO
  private:
    cl::Platform platform_;
    cl::Device device_;
    cl::Context context_;
    cl::CommandQueue queue_;

  public:
};

#endif // INCLUDE_GPU_CONTEXT_HPP