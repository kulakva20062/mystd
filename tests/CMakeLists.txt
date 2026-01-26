include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(logger_test
    logger_test.cpp
)

target_link_libraries(logger_test
    PRIVATE
    mystd_logers
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(logger_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

include(GoogleTest)
gtest_discover_tests(logger_test
    DISCOVER_TESTS_OPTIONS
        "--gtest_list_tests"
    PROPERTIES
        LABELS "Logger"
)
