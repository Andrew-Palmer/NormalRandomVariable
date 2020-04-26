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

install(TARGETS NormalRandomVariable DESTINATION /usr/local/lib EXPORT NormalRandomVariableTargets)
install(FILES include/NormalRandomVariable/NormalRandomVariable.h DESTINATION /usr/local/include)
install(EXPORT NormalRandomVariableTargets FILE NormalRandomVariableTargets.cmake DESTINATION /usr/local/lib/cmake/NormalRandomVariable)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/NormalRandomVariableConfig.cmake"
  INSTALL_DESTINATION "/usr/local/lib/cmake/NormalRandomVariable"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

# install the configuration file
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/NormalRandomVariableConfig.cmake
  DESTINATION /usr/local/lib/cmake/NormalRandomVariable
  )