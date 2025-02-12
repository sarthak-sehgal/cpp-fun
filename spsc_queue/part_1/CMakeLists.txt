cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    CircularBuffer
    VERSION 1.0
    LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.3.11
    GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(nanobench)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE nanobench)
