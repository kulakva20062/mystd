cmake_minimum_required(VERSION 4.2.2)
project(mystd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# --- LibTorch ---
include(FetchContent)

set(LIBTORCH_VERSION "2.5.1")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(LIBTORCH_URL
        "https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-${LIBTORCH_VERSION}%2Bcpu.zip"
    )
else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

FetchContent_Declare(
    libtorch
    URL ${LIBTORCH_URL}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(libtorch)

list(APPEND CMAKE_PREFIX_PATH "${libtorch_SOURCE_DIR}")
find_package(Torch REQUIRED)

# --- Boost ---
find_package(Boost REQUIRED)

# --- Qt6 ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Проект ---
add_subdirectory(lib)

add_executable(mystd bin/main.cpp)
target_link_libraries(mystd PRIVATE mystd_lib ${TORCH_LIBRARIES} Qt6::Widgets Boost::headers)
target_compile_options(mystd PRIVATE ${TORCH_CXX_FLAGS})

enable_testing()
add_subdirectory(tests)