cmake_minimum_required(VERSION 3.15)
project(mystd VERSION 1.0.0 LANGUAGES CXX)

# Настройки C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Директории для сборки
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Включить тестирование
enable_testing()

# Добавить поддиректории если они существуют
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    add_subdirectory(lib)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif()