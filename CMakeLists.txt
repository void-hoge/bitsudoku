cmake_minimum_required(VERSION 3.10)
project(bitsudoku)

set(CMAKE_CXX_STANDARD 20)

add_executable(bitsudoku main.cpp board.cpp solver.cpp)
add_executable(sudokugen sudokugen.cpp board.cpp solver.cpp generator.cpp)
# add_executable(test test.cpp board.cpp)

set("CMAKE_CXX_FLAGS" "-O3 -mtune=native -march=native")
