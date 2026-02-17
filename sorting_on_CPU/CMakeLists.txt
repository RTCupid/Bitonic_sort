cmake_minimum_required(VERSION 3.16)

project(sort_cpu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SORT_CPU_SOURCES src/main.cpp)
set(SORT_CPU_INCLUDES include)

add_executable(cpu ${SORT_CPU_SOURCES})
target_include_directories(cpu PRIVATE ${SORT_CPU_INCLUDES})

add_executable(cpu_time ${SORT_CPU_SOURCES})
target_include_directories(cpu_time PRIVATE ${SORT_CPU_INCLUDES})
target_compile_definitions(cpu_time PRIVATE TIME_TEST)

enable_testing()
add_subdirectory(tests)
