cmake_minimum_required(VERSION 4.2.2)
project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)