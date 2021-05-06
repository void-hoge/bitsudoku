cmake_minimum_required(VERSION 3.16)
project(bitsudoku)

set(CMAKE_CXX_STANDARD 17)

add_executable(bitsudoku main.cpp board.cpp solver.cpp)
#set(CMAKE_CXX_COMPILER "gcc-10")