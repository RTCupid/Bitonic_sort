cmake_minimum_required(VERSION 3.16)

project(sorting_on_CPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ON_CPU sorting_on_CPU)
set(ON_CPU_TIME sorting_on_CPU_time)

add_library(range_queries_headers INTERFACE)
target_include_directories(range_queries_headers
    INTERFACE ${CMAKE_SOURCE_DIR}/include
)
add_library(range_queries::headers ALIAS range_queries_headers)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

function(add_sorting_executable TARGET_NAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs DEFINITIONS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${TARGET_NAME} PRIVATE range_queries::headers)

    if(ARG_DEFINITIONS)
        target_compile_definitions(${TARGET_NAME} PRIVATE ${ARG_DEFINITIONS})
    endif()
endfunction()

add_sorting_executable(${ON_CPU} ${SOURCES} DEFINITIONS ON_CPU_RUN)
add_sorting_executable(${ON_CPU_TIME} ${SOURCES} DEFINITIONS ON_CPU_TIME_RUN)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${ON_CPU} PRIVATE -mllvm -inline-threshold=500)
    target_compile_options(${ON_CPU_TIME} PRIVATE -mllvm -inline-threshold=500)
endif()

enable_testing()
#add_subdirectory(tests)