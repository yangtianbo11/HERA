cmake_minimum_required(VERSION 3.17)
project(Algorithms_C++)

set(CMAKE_CXX_STANDARD 20)

add_executable(Algorithms_C__ main
         stringAnd_HOF_Examples
        factorial
        fibonacci.cpp fibonacci.h)

include_directories(../HaverfordCS/include)
