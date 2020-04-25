cmake_minimum_required(VERSION 3.10)

project(NormalRandomVariableExample VERSION 1.0.0 LANGUAGES CXX)

find_package(NormalRandomVariable)

add_executable(example example.cpp)
