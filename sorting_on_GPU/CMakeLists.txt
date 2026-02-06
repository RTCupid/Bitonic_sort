cmake_minimum_required(VERSION 3.16)

project(sorting_on_GPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCL REQUIRED)

message(STATUS "OpenCL библиотеки: ${OpenCL_LIBRARIES}")
message(STATUS "OpenCL заголовки: ${OpenCL_INCLUDE_DIRS}")


target_link_libraries(sorting_gpu ${OpenCL_LIBRARIES})
target_include_directories(sorting_gpu PRIVATE ${OpenCL_INCLUDE_DIRS})

target_include_directories(sorting_gpu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
#add_subdirectory(tests)
