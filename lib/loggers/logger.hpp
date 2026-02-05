#pragma once
#include "../file_manager/formats/file_log.hpp"
#include "../time/time.hpp"
#include <ctime>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

namespace mystd {

    namespace {
        constexpr size_t DEFAULT_LEVEL_LOG = 0;
        constexpr size_t DEFAULT_MAX_FILES_LOG = 10;
        constexpr size_t DEFAULT_LEVEL_CLEAN_LOG = 0;
    }

    class Logger final {
        public:
            Logger(const fs::path& file_path = "logs/log-" + std::to_string(mystd::get_year())
            + "-" + std::to_string(mystd::get_month()) + "-" + std::to_string(mystd::get_day())
            + "_" + std::to_string(mystd::get_hour()) + ":" + std::to_string(mystd::get_minute())
            + ":" + std::to_string(mystd::get_second()) + ".log");

            ~Logger() = default;

            Logger(const Logger& other) = delete;

            Logger& operator=(const Logger& other) = delete;

            void Log(std::string_view message, size_t level = DEFAULT_LEVEL_LOG);

            void CleanLogs(size_t max_files = DEFAULT_MAX_FILES_LOG, size_t level_clean = DEFAULT_LEVEL_CLEAN_LOG);

        private:
            FileLog file_txt_;
    };

} // namespace mystd