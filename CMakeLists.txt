cmake_minimum_required(VERSION 3.23)
project(Task1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

add_executable(Task1 Calc_Sp_and_Ef.cpp)
