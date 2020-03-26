# Locate GTest
find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

add_executable(nrv_test nrv_test.cpp)
target_link_libraries(nrv_test NormalRandomVariable GTest::Main)