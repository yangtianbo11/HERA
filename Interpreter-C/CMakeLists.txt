cmake_minimum_required(VERSION 3.1)
project(Interpreter_C++)

set(CMAKE_CXX_STANDARD 20)

add_executable(Interpreter_C++
  scanner-regexp
  scanner
  scannerDemo
  parser
  Tree
        TreeHelpers
        TreeHelpersHOF
        Dictionary
  eval
  main
        Tree.cpp)

include_directories(../HaverfordCS/include)
