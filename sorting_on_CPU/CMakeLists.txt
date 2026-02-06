cmake_minimum_required(VERSION 3.16)

project(sorting_on_CPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(sorting_on_CPU
    src/main.cpp
)

target_include_directories(sorting_on_CPU PRIVATE
    include
)

enable_testing()
#add_subdirectory(tests)