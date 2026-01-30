#pragma once

#include <cstddef>
#include <cstdint>

namespace mystd {

    std::size_t get_year(); //size_t потому что не знаем какой может быть год   

    std::uint8_t get_month();  

    std::uint8_t get_day();   

    std::uint8_t get_hour();   

    std::uint8_t get_minute(); 

    std::uint8_t get_second(); 

}  // namespace mystd
