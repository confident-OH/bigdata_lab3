cmake_minimum_required(VERSION 3.17)
project(Merkle)

set(CMAKE_CXX_STANDARD 14)

add_executable(Merkle main.cpp M_tree.cpp M_tree.h sha256.h sha256.cpp)