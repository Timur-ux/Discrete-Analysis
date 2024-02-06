cmake_minimum_required(VERSION 3.10)

project(
    Discrete_Analysis
    CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g)

add_executable(main main.cpp)
target_link_directories(main PUBLIC Sorts)