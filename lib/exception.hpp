#pragma once
#include <exception>

namespace mystd 
{
    class Exception :public std::exception {
        public:
            explicit Exception(const char* text_error, size_t error) noexcept;

            virtual ~Exception() noexcept override;

            Exception(const Exception& other) noexcept;

            Exception& operator=(const Exception& other) noexcept;

            virtual const char* what() const noexcept override;

            virtual size_t GetError() const noexcept;
        protected:
            const char* text_error;
            size_t error;
    };

} //namespace mystd