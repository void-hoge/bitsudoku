cmake_minimum_required(VERSION 3.10)
project(bitsudoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(bitsudoku main.cpp board.cpp solver.cpp)
add_executable(sudokugen test.cpp board.cpp solver.cpp generator.cpp)
set(CMAKE_CXX_COMPILER "g++-11")

set("CMAKE_CXX_FLAGS" "-O3 -mtune=native -march=native")
