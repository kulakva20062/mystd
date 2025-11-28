cmake_minimum_required(VERSION 3.20)

project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)
