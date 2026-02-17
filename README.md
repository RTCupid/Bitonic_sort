<div align="center">

# Implementation of Bitonic Sort in C++ with GPU computation
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![OpenCL](https://img.shields.io/badge/OpenCL-Accelerated-blue?logo=opencl)

</div>

## README in other languages 

1. [Russian](/README-R.md)
2. [English](/README.md)

## Table of Contents
Introduction:
- [Building The Program](#building-the-program)
- [Program Execution Targets](#program-execution-targets)
- [Format Of Input Data](#format-of-input-data)
- [Introduction](#introduction)
- [Bitonic Sort Algorithm](#bitonic-sort-algorithm)
- [Project Structure](#project-structure)
- [Project Authors](#project-authors)

### Building The Program
Repository cloning, build and compilation is performed using the following commands:

```
git clone git@github.com:RTCupid/Bitonic_sort.git
cd Bitonic_sort
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Program Execution Targets

After building the project, navigate to the `build` directory and run one of the following executables:

| Executable | Description | Output |
| :--- | :--- | :--- |
| `./gpu` | Runs the Bitonic sort algorithm on the **GPU** | Displays the sorted sequence |
| `./cpu` | Runs the Bitonic sort algorithm on the **CPU** | Displays the sorted sequence |
| `./gpu_time` | Measures execution time for **GPU sorting** | Shows elapsed time only |
| `./cpu_time` | Measures execution time for **CPU sorting** | Shows elapsed time only |

### Usage Example

```bash
cd build
./gpu        # Verify correctness on GPU
./cpu        # Verify correctness on CPU
./gpu_time   # Benchmark GPU performance
./cpu_time   # Benchmark CPU performance
```

## Format Of Input Data
  `Number` of numbers in sequence and than `n` numbers.

## Introduction

Sorting data is one of the most fundamental tasks in computer science. In many real-world applications, organizing information in a specific order is crucial for efficient processing. For example, consider a financial system that needs to analyze thousands of transactions per second. To detect fraud or generate reports, these transactions must be sorted by timestamp quickly and accurately.

Traditionally, developers rely on CPU-based algorithms like QuickSort or MergeSort. While these methods are reliable for small datasets, they have significant limitations for large-scale tasks. CPUs typically have a limited number of cores, and processing massive arrays sequentially can become a performance bottleneck. As the volume of data increases, the time required for sorting grows, leading to slower application response times.

To overcome these limitations, this project leverages GPU acceleration using the Bitonic Sort algorithm. Modern Graphics Processing Units contain thousands of cores designed for parallel computation. Bitonic Sort is particularly well-suited for this architecture because it performs comparisons in a fixed pattern, allowing many operations to execute simultaneously. This approach offers a significant performance advantage, making it a highly efficient solution for high-performance computing tasks.

## Bitonic Sort Algorithm

Bitonic Sort is a parallel sorting algorithm designed to sort sequences by constructing and merging bitonic sequences. A bitonic sequence is a sequence that first increases and then decreases (or vice versa). The algorithm follows these key steps:

1.  **Preparation:** The algorithm requires the input size to be a power of two. If the data does not meet this requirement, the array is padded with neutral values to reach the nearest power of two.
2.  **Creating Bitonic Sequences:** The data is recursively divided into smaller chunks. One half is sorted in ascending order, while the other half is sorted in descending order. This creates a bitonic sequence.
3.  **Compare and Swap:** Elements are compared at specific distances across the sequence. If two elements are out of order according to the desired direction, they are swapped. Crucially, this step is performed in parallel for all pairs.
4.  **Recursive Splitting:** The sequence is split into two halves, and the process repeats until each subsequence contains only one element. At this stage, the entire array is fully sorted.

Because the comparison pattern is fixed and does not depend on the data values, the GPU can execute all operations simultaneously without waiting for previous results. This maximizes hardware utilization and ensures consistent performance.

## Project Structure

<details>
<summary>Project structure</summary>

```
.
├── benchmark.py
├── CMakeLists.txt
├── sorting_on_CPU
│   ├── CMakeLists.txt
│   ├── include
│   │   └── utils.hpp
│   ├── src
│   │   └── main.cpp
│   └── tests
│       ├── CMakeLists.txt
│       └── end_to_end
│           ├── CMakeLists.txt
│           └── end_to_end.sh
├── sorting_on_GPU
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── bitonic.hpp
│   │   ├── buffer.hpp
│   │   ├── gpu_context.hpp
│   │   ├── kernel.cl
│   │   ├── kernel.hpp
│   │   └── utils_gpu.hpp
│   ├── src
│   │   └── main.cpp
│   └── tests
│       ├── CMakeLists.txt
│       └── end_to_end
│           ├── CMakeLists.txt
│           └── end_to_end.sh
└── tests
    ├── answ
    ├── data
    └── generator.py

```

</details>

## Project Authors

<div align="center">

  <a href="https://github.com/RTCupid">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/A.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <a href="https://github.com/BulgakovDmitry">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/D.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <br>
  <a href="https://github.com/RTCupid"><strong>@RTCupid, </strong></a>
  <a href="https://github.com/BulgakovDmitry"><strong>@BulgakovDmitry, </strong></a>
  <br>
</div>
