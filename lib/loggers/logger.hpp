#pragma once
#include "../file_manager/formats/file_txt.hpp"
#include <filesystem>

namespace fs = std::filesystem;

class Logger final {
    public:
        Logger(const fs::path& file_path);

        ~Logger() = default;

        Logger(const Logger& other) = delete;

        Logger& operator=(const Logger& other) = delete;

        void Log(const std::string& message);

    private:
        FileTxt file_txt_;
};