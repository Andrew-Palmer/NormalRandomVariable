cmake_minimum_required(VERSION 3.10)

project(NormalRandomVariable)

option(BUILD_TESTS "Build the tests" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(test)

    enable_testing()

    add_test(
        NAME nrv_test
        COMMAND nrv_test
    )
endif(BUILD_TESTS)