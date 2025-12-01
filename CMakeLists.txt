cmake_minimum_required(VERSION 4.0)
project(P1_Project C)

set(CMAKE_C_STANDARD 11)

#Include mtest dependencies
include(cmake/mtest.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
add_subdirectory(SimulationBenchmark)