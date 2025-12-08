include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    FIND_PACKAGE_ARGS NAMES GTest
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    mystd_tests
    vector_tests.cpp
    parser_tests.cpp
)

enable_testing()

target_link_libraries(
    mystd_tests
    PRIVATE
        GTest::gtest    
        GTest::gtest_main
        mystd
)

target_compile_definitions(
    mystd_tests
    PRIVATE
        RESOURCES_DIR=\"${CMAKE_SOURCE_DIR}/tests/resources\"
        MYSTD_EXE_PATH=\"$<TARGET_FILE:mystd>\"
)

include(GoogleTest)
gtest_discover_tests(mystd_tests)
