cmake_minimum_required(VERSION 3.10)

project(
    Discrete_Analysis
    CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g)

add_executable(main main.cpp)
include_directories(
  PUBLIC Sorts
  PUBLIC SearchingTree
  PUBLIC Tries
  PUBLIC Utils
  PUBLIC Strings
)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES FALSE tests/*.cpp)
file(GLOB_RECURSE STRING_SOURCES LIST_DIRECTORIES FALSE Strings/src/*.cpp)

add_library(strings_lib STATIC ${STRING_SOURCES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

add_executable(main_test ${TEST_SOURCES})
target_include_directories(main_test PUBLIC Tries)
target_link_libraries(main_test
  ${GTEST_LIBRARIES}
  strings_lib)
target_link_libraries(main strings_lib)

