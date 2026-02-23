include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(bit_test bit_test.cpp)
target_include_directories(bit_test PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(bit_test PRIVATE GTest::gtest GTest::gtest_main)

add_executable(bit_test_be bit_test.cpp)
target_include_directories(bit_test_be PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(bit_test_be PRIVATE GTest::gtest GTest::gtest_main)
target_compile_definitions(bit_test_be PRIVATE
    __BYTE_ORDER__=4321
    __ORDER_BIG_ENDIAN__=4321
    __ORDER_LITTLE_ENDIAN__=1234
)

add_executable(arg_parser_test
    arg_parser_test.cpp
)

target_link_libraries(arg_parser_test
    PRIVATE
    mystd_parsers
    mystd_exception
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(arg_parser_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

include(GoogleTest)

gtest_discover_tests(bit_test PROPERTIES LABELS "Bit")
gtest_discover_tests(bit_test_be PROPERTIES LABELS "Bit;BigEndian")
gtest_discover_tests(arg_parser_test
    DISCOVER_TESTS_OPTIONS
        "--gtest_list_tests"
    PROPERTIES
        LABELS "ArgParser"
)
