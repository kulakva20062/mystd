#pragma once
#include "vector.hpp"

namespace mystd {
    
    class String : public Vector<char> {
        public:
            explicit String(size_t size = 0, char default_value = '\0');

            ~String() override;

            String(const String& other);
            
            String(const Vector<char>& other);

            String(const char* other);
            
            String(const char other);
            
            String& operator=(const String& other);
            
            String& operator+=(const String& other);
            
            String operator+(const String& other) const;

            void Upper(const size_t begin, const size_t end);

            void Lower(const size_t begin, const size_t end);

            void Upper();

            void Lower();

            int64_t ToInt64();
            
            String SubStr(const size_t begin, const size_t end) const;
            
    };
    
    bool operator==(const String& lhs, const String& rhs);
    
    bool operator!=(const String& lhs, const String& rhs);

}  // namespace mystd
