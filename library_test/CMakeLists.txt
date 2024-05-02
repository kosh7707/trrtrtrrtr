cmake_minimum_required(VERSION 3.26)
project(library_test)

set(CMAKE_CXX_STANDARD 17)

add_library(library_test SHARED library.cpp)
