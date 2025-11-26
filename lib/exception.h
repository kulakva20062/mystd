#pragma once
#include <exception>

namespace mystd 
{
    class Exception :public std::exception {
        public:
            explicit Exception(const char* text, size_t error);

            virtual ~Exception() override;

            Exception(const Exception& other);

            Exception& operator=(const Exception& other);

            const char* GetTextError();

            size_t GetError();
        protected:
            const char* text_error;
            size_t error;
    };

} //namespace mystd