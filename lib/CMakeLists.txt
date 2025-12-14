add_library(mystd
    string.cpp
    parser.cpp
    file_helper.cpp
    exception.cpp
)

target_include_directories(mystd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(mystd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})