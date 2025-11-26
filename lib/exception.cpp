#include "exception.hpp"

namespace mystd {

    Exception::Exception(const char* text_error, size_t error) noexcept 
        :error(error)
        ,text_error(text_error)
    {}

    Exception::~Exception() noexcept {}

    Exception::Exception(const Exception& other) noexcept
        :error(other.error)
        ,text_error(other.text_error)
    {}

    Exception& Exception::operator=(const Exception& other) noexcept {
        if (&other == this) {
            return *this;
        }
        error = other.error;
        text_error = other.text_error;
        return *this;
    }   

    const char* Exception::what() const noexcept {
        return text_error;
    }

    size_t Exception::GetError() const noexcept {
        return error;
    }

}