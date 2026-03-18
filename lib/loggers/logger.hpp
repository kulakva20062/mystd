#pragma once
#include "../file_manager/formats/file_log.hpp"
#include <ctime>
#include <filesystem>
#include <string_view>
#include <string>

namespace fs = std::filesystem;

namespace mystd {

    namespace {
        constexpr size_t DEFAULT_LEVEL_LOG = 0;
        constexpr size_t DEFAULT_MAX_FILES_LOG = 10;
        constexpr size_t DEFAULT_LEVEL_CLEAN_LOG = 0;
        static std::time_t time_old = std::time(nullptr);
   }

    class Logger final {
        public:
        Logger(const fs::path& file_path = "logs/" + []{ std::string s = std::ctime(&time_old); s.erase(s.find('\n')); return s; }() + ".log");

            ~Logger() = default;

            Logger(const Logger& other) = delete;

            Logger& operator=(const Logger& other) = delete;

            void Log(std::string_view message, size_t level = DEFAULT_LEVEL_LOG);

            void CleanLogs(size_t max_files = DEFAULT_MAX_FILES_LOG, size_t level_clean = DEFAULT_LEVEL_CLEAN_LOG);

        private:
            FileLog file_txt_;
    };

} // namespace mystd