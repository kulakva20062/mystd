#include "logger.hpp"
#include <filesystem>

namespace fs = std::filesystem;

Logger::Logger(const fs::path& file_path)
    :file_txt_(file_path)
{
    file_txt_.Clear();
}

void Logger::Log(const std::string& message)
{
    file_txt_ += message;
}
