cmake_minimum_required(VERSION 3.10)
project(bitsudoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(bitsudoku main.cpp board.cpp solver.cpp)
set(CMAKE_CXX_COMPILER "g++-11")

set("CMAKE_CXX_FLAGS" "-O2 -mtune=native -march=native")
