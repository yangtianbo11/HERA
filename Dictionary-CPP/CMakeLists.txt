cmake_minimum_required(VERSION 3.1)
project(Dictionary_C++)

set(CMAKE_CXX_STANDARD 20)

add_executable(Dictionary_C++ main
       	listDemos
	listPractice
	test-Dictionary	
	streams
	Calculator Dictionary.cpp Dictionary.h)

include_directories(../HaverfordCS/include)
