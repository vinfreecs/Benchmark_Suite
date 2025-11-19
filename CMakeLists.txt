cmake_minimum_required(VERSION 3.15)

project(Basic_Benchmark_suite)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
find_package(OpenMP REQUIRED COMPONENTS CXX)
add_executable(benchmark src/main.cpp src/timing.c)

target_link_libraries(benchmark PRIVATE OpenMP::OpenMP_CXX)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")