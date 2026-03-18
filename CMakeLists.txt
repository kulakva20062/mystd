cmake_minimum_required(VERSION 4.2.2)
project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Отключаем clang-scan-deps (CMake 4.x), из-за него clang не находит stddef.h
set(CMAKE_DEPENDS_USE_COMPILER FALSE)

# --- Проект ---
add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)