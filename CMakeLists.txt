cmake_minimum_required(VERSION 4.2.2)
project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)

add_executable(mystd bin/main.cpp)
target_link_libraries(mystd PRIVATE mystd_lib)

enable_testing()
add_subdirectory(tests)