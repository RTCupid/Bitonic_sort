cmake_minimum_required(VERSION 3.16)

project(sort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sorting_gpu
    sorting_on_GPU/src/main.cpp
)

add_executable(sorting_cpu
    sorting_on_CPU/src/main.cpp
)

target_include_directories(sorting_gpu PRIVATE
    include
)

target_include_directories(sorting_cpu PRIVATE
    include
)

enable_testing()
add_subdirectory(sorting_on_CPU)
add_subdirectory(sorting_on_GPU)
