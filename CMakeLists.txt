cmake_minimum_required(VERSION 3.20)
project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER g++)

# Создайте библиотеки
add_subdirectory(lib)

# Исполняемый файл
add_executable(mystd bin/main.cpp)
target_link_libraries(mystd PRIVATE mystd_lib)

# Тесты (если есть)
enable_testing()
add_subdirectory(tests)