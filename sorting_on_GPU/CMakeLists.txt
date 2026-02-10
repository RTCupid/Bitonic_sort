cmake_minimum_required(VERSION 3.16)

project(sort_gpu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCL REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCL_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCL_INCLUDE_DIRS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)

enable_testing()
add_subdirectory(tests)
