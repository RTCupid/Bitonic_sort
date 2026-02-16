cmake_minimum_required(VERSION 3.16)

project(sort_gpu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCL REQUIRED)
if(NOT OpenCL_FOUND)
    message(WARNING "OpenCL not found, building without GPU support")
    add_definitions(-DNO_OPENCL)
endif()

set(SORT_GPU_SOURCES src/main.cpp)
set(SORT_GPU_INCLUDES ${OpenCL_INCLUDE_DIRS} include)
set(SORT_GPU_LIBS ${OpenCL_LIBRARIES})

add_executable(gpu ${SORT_GPU_SOURCES})
target_compile_definitions(gpu PRIVATE CL_HPP_ENABLE_EXCEPTIONS)
target_link_libraries(gpu PRIVATE ${SORT_GPU_LIBS})
target_include_directories(gpu PRIVATE ${SORT_GPU_INCLUDES})

add_executable(gpu_time ${SORT_GPU_SOURCES})
target_compile_definitions(gpu_time PRIVATE CL_HPP_ENABLE_EXCEPTIONS TIME_TEST)
target_link_libraries(gpu_time PRIVATE ${SORT_GPU_LIBS})
target_include_directories(gpu_time PRIVATE ${SORT_GPU_INCLUDES})

enable_testing()
add_subdirectory(tests)
