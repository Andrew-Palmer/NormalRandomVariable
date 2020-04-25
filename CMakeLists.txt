cmake_minimum_required(VERSION 3.10)

project(NormalRandomVariable VERSION 1.0.0 LANGUAGES CXX)

########################################################################################
# Create target and set properties

add_library(NormalRandomVariable src/NormalRandomVariable.cpp)

target_include_directories(NormalRandomVariable 
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_options(NormalRandomVariable PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(NormalRandomVariable PRIVATE cxx_std_11)

########################################################################################
# Tests

option(BUILD_TESTS "Build the tests" OFF)

if(BUILD_TESTS)
    add_subdirectory(test)

    enable_testing()

    add_test(
        NAME nrv_test
        COMMAND nrv_test
    )
endif(BUILD_TESTS)

########################################################################################
# Installation

install(TARGETS NormalRandomVariable DESTINATION /usr/local/lib)
install(FILES include/NormalRandomVariable/NormalRandomVariable.h DESTINATION /usr/local/include)
