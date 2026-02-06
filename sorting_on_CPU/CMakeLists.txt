cmake_minimum_required(VERSION 3.16)

project(sorting_on_CPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ON_CPU sorting_on_CPU_run)
set(ON_CPU_TIME sorting_on_CPU_time_run)

set(SOURCES
    src/main.cpp
)

add_library(sorting_on_CPU INTERFACE)
target_include_directories(sorting_on_CPU
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
add_library(sorting_on_CPU::headers ALIAS sorting_on_CPU)

add_executable(${ON_CPU} ${SOURCES})
target_link_libraries(${ON_CPU} PRIVATE sorting_on_CPU)

add_executable(${ON_CPU_TIME} ${SOURCES})
target_link_libraries(${ON_CPU_TIME} PRIVATE sorting_on_CPU)

target_compile_definitions(${ON_CPU_TIME} PRIVATE ON_CPU_TIME)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${ON_CPU} PRIVATE -mllvm -inline-threshold=500)
    target_compile_options(${ON_CPU_TIME} PRIVATE -mllvm -inline-threshold=500)
endif()

enable_testing()
#add_subdirectory(tests)
