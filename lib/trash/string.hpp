#pragma once
#include "vector.hpp"

namespace mystd {
    
    class String final : public Vector<char> {
        public:
            explicit String(size_t size = 0, char default_value = '\0');

            ~String() override;

            String(const String& other);
            
            explicit String(const Vector<char>& other);

            String(const char* other);
            
            String(const char other);
            
            String& operator=(const String& other);
            
            String& operator+=(const String& other);
            
            String operator+(const String& other) const;

            void Upper(const size_t begin, const size_t end);

            void Lower(const size_t begin, const size_t end);

            void Upper();

            void Lower();

            String SubStr(const size_t begin, const size_t end) const;
            
            const char* CStr() const;
            
            int64_t ToInt() const;
            
            long double ToDouble() const;
            
    };
    
    bool operator==(const String& lhs, const String& rhs);
    
    bool operator!=(const String& lhs, const String& rhs);
    
    std::ostream& operator<<(std::ostream& os, const String& str);
    
    std::istream& operator>>(std::istream& is, String& str);

}  // namespace mystd
