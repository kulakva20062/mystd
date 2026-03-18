include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(bit_test bit_test.cpp)
target_link_libraries(bit_test PRIVATE mystd_lib GTest::gtest GTest::gtest_main)
gtest_discover_tests(bit_test PROPERTIES LABELS "Bit")

add_executable(bit_test_be bit_test.cpp)
target_link_libraries(bit_test_be PRIVATE mystd_lib GTest::gtest GTest::gtest_main)
target_compile_definitions(bit_test_be PRIVATE
    __BYTE_ORDER__=4321
    __ORDER_BIG_ENDIAN__=4321
    __ORDER_LITTLE_ENDIAN__=1234
)
gtest_discover_tests(bit_test_be PROPERTIES LABELS "Bit;BigEndian")

add_executable(bor_test bor_test.cpp)
target_link_libraries(bor_test PRIVATE mystd_lib GTest::gtest GTest::gtest_main)
gtest_discover_tests(bor_test PROPERTIES LABELS "Bor")

add_executable(arg_parser_test arg_parser_test.cpp)
target_link_libraries(arg_parser_test PRIVATE mystd_parsers GTest::gtest GTest::gtest_main)
gtest_discover_tests(arg_parser_test PROPERTIES LABELS "ArgParser")
