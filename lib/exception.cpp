#include "exception.h"

namespace mystd 
{
    Exception::Exception(const char* text_error, size_t error)
        :error(error)
        ,text_error(text_error)
    {}

    Exception::~Exception() {}

    Exception::Exception(const Exception& other) {
        error = other.error;
        text_error = other.text_error;
    }

    Exception& Exception::operator=(const Exception& other) {
        if (&other == this) {
            return *this;
        }
        error = other.error;
        text_error = other.text_error;
        return *this;
    }   

    const char* Exception::GetTextError() {
        return text_error;
    }

    size_t Exception::GetError() {
        return error;
    }

} //namespace mystd