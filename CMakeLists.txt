cmake_minimum_required(VERSION 3.20)
project(bmaisarvore)

set(CMAKE_CXX_STANDARD 14)

add_executable(bmaisarvore main.cpp BpTree.cpp BpTree.h Node.cpp Node.h)
