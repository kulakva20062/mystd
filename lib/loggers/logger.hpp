#pragma once
#include "../file_manager/formats/file_txt.hpp"
#include "../time/time.hpp"
#include <filesystem>

namespace fs = std::filesystem;

class Logger final {
    public:
        Logger(const fs::path& file_path = "logs/log-" + std::to_string(mystd::get_year())
         + "-" + std::to_string(mystd::get_month()) + "-" + std::to_string(mystd::get_day())
         + "_" + std::to_string(mystd::get_hour()) + ":" + std::to_string(mystd::get_minute())
         + ":" + std::to_string(mystd::get_second()) + ".txt");

        ~Logger() = default;

        Logger(const Logger& other) = delete;

        Logger& operator=(const Logger& other) = delete;

        void Log(const std::string& message);

    private:
        FileTxt file_txt_;
};