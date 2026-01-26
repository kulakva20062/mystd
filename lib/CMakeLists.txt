add_library(mystd_exception
    exception.hpp
    exception.cpp
)
target_include_directories(mystd_exception PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(mystd_containers
    containers/deque.hpp
    containers/queue.hpp
    containers/string.hpp
    containers/string.cpp
    containers/vector.hpp
)
target_include_directories(mystd_containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mystd_containers PUBLIC mystd_exception)

add_library(mystd_parsers
    parsers/arg_parser.hpp
    parsers/arg_parser.cpp
)
target_include_directories(mystd_parsers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mystd_parsers PUBLIC mystd_containers)

add_library(mystd_file_manager
    file_manager/dir_file.hpp
    file_manager/dir_file.cpp
    file_manager/file.hpp
    file_manager/file.cpp
    file_manager/regular_file.hpp
    file_manager/regular_file.cpp
    file_manager/formats/file_txt.hpp
    file_manager/formats/file_txt.cpp
)
target_include_directories(mystd_file_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mystd_file_manager PUBLIC mystd_exception)

add_library(mystd_logers
    loggers/logger.hpp
    loggers/logger.cpp
)
target_include_directories(mystd_logers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mystd_logers PUBLIC mystd_file_manager)

add_library(mystd_lib INTERFACE)
target_link_libraries(mystd_lib INTERFACE 
    mystd_exception
    mystd_containers
    mystd_parsers
    mystd_file_manager
    mystd_logers
)
target_include_directories(mystd_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})