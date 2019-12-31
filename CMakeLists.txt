cmake_minimum_required(VERSION 3.15)
project(new_vm)

set(CMAKE_CXX_STANDARD 17)
add_executable(new_vm main.cpp compile_random.h nn_integer.h nand_nor.h utils.h)

project(tests)
find_package(GTest CONFIG REQUIRED)
add_executable(tests tests.cpp compile_random.h nn_integer.h nand_nor.h)
target_include_directories(tests PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(tests GTest::gtest GTest::gtest_main ${ADDITIONAL_LIBRARIES})