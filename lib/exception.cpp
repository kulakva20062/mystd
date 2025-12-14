#include "exception.hpp"

namespace mystd 
{
    Exception::Exception(const char* text_error, size_t error) 
            :error(error)
            ,text_error(text_error)
    {} 
    
    const char* Exception::what() const noexcept{
        return text_error;
    }
    
    size_t Exception::GetError() const {
        return error;
    }
} //namespace mystd