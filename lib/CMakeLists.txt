
add_library(mystd
    exception.cpp 
    exception.hpp 
    vector.hpp
    string.cpp
    string.hpp
)

target_include_directories(mystd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(mystd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})