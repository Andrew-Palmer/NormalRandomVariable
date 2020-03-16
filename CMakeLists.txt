cmake_minimum_required(VERSION 3.10)

# set the project name
project(NormalRandomVariable)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD _REQUIRED True)

add_subdirectory(src)
add_subdirectory(test)