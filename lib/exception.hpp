#pragma once
#include <exception>

namespace mystd 
{
    class Exception :public std::exception {
        public:
            explicit Exception(const char* text_error, size_t error) noexcept 
                    :error(error)
                    ,text_error(text_error)
            {} 

            virtual ~Exception() noexcept override {};

            Exception(const Exception& other) noexcept
                :error(other.error)
                ,text_error(other.text_error)
            {}  

            Exception& operator=(const Exception& other) noexcept {
                if (&other == this) {
                    return *this;
                }
                error = other.error;
                text_error = other.text_error;
                return *this;
            }   

            virtual const char* what() const noexcept override {
                return text_error;
            }

            virtual size_t GetError() const noexcept {
                return error;
            }
            
        protected:
            const char* text_error;
            size_t error;
    };

} //namespace mystd