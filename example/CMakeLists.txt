cmake_minimum_required(VERSION 3.0)
project(gnuplot_example)

add_executable(example_out example.cpp)
target_include_directories(example_out PRIVATE ../include)