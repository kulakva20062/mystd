#include "time.hpp"
#include <ctime>

namespace mystd {

    std::size_t get_year() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return tm ? static_cast<std::size_t>(tm->tm_year) + 1900 : 0;
    }

    std::uint8_t get_month() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return tm ? static_cast<std::uint8_t>(tm->tm_mon + 1) : 0;
    }

    std::uint8_t get_day() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return tm ? static_cast<std::uint8_t>(tm->tm_mday) : 0;
    }

    std::uint8_t get_hour() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return tm ? static_cast<std::uint8_t>(tm->tm_hour) : 0;
    }

    std::uint8_t get_minute() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return tm ? static_cast<std::uint8_t>(tm->tm_min) : 0;
    }

    std::uint8_t get_second() {
        std::time_t t = std::time(nullptr);
        std::tm* tm = std::localtime(&t);
        return tm ? static_cast<std::uint8_t>(tm->tm_sec) : 0;
    }

}  // namespace mystd
