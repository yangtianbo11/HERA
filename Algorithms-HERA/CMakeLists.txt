cmake_minimum_required(VERSION 3.1)
project(Algorithms_HERA)

set(CMAKE_CXX_STANDARD 20)

add_executable(Algorithms_HERA HERA_main)

include_directories(../HERA-C)
