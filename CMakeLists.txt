cmake_minimum_required(VERSION 3.14)
project(mystd)

# Включить Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9fdab5977a8ad7f2b6ed44c.zip
)

FetchContent_MakeAvailable(googletest)

# Поддиректории
add_subdirectory(lib)
add_subdirectory(tests)