add_library(mystd 
    # Исключения
    exception.hpp
    exception.cpp
    
    # Контейнеры
    containers/deque.hpp
    containers/queue.hpp
    containers/string.hpp
    containers/string.cpp
    containers/vector.hpp
    
    # Парсеры
    parsers/arg_parser.hpp
    parsers/arg_parser.cpp
    
    # Файловый менеджер
    file_manager/dir_file.hpp
    file_manager/dir_file.cpp
    file_manager/file.hpp
    file_manager/file.cpp
    file_manager/regular_file.hpp
    file_manager/regular_file.cpp
)

# Устанавливаем include директорию так, чтобы можно было подключать файлы как:
# #include "string.hpp" или #include "containers/string.hpp"
# #include "parsers/arg_parser.hpp"
# #include "file_manager/file.hpp"
target_include_directories(mystd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})