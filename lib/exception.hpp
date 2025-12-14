#pragma once
#include <exception>

namespace mystd 
{
    class Exception :public std::exception {
        public:
            explicit Exception(const char* text_error, size_t error);

            virtual ~Exception() override = default;

            Exception(const Exception& other) = default;

            Exception& operator=(const Exception& other) = default;

            virtual const char* what() const noexcept override;

            virtual size_t GetError() const;
            
        protected:
            const char* text_error;
            size_t error;
    };

} //namespace mystd