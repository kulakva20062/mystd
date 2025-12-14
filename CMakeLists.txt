cmake_minimum_required(VERSION 3.20)
project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER g++)

# Добавьте lib в include пути
include_directories(lib)

# Создайте библиотеку
add_subdirectory(lib)

# Исполняемый файл
add_executable(Testlib bin/main.cpp)
target_link_libraries(Testlib PRIVATE mystd)

# Тесты (если есть)
enable_testing()
add_subdirectory(tests)