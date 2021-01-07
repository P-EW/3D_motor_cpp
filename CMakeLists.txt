# cmake_minimum_required(VERSION <specify CMake version here>)
project(3D_motor_cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(3D_motor_cpp main.cpp libs/tgaimage.cpp libs/tgaimage.h)