#include "string.hpp"
#include "vector.hpp"
#include <cstring>
#include <cstdint>

namespace mystd {

    String::String(size_t size, char default_value) : mystd::Vector<char> (size, default_value) {}

    String::~String() {}

    String::String(const String& other) : mystd::Vector<char> (other) {}

    String::String(const char* other) : mystd::Vector<char> (strlen(other))
    {
        for (size_t index = 0; index < Size(); ++index) {
            operator[](index) = other[index];
        }
    }

    String::String(const char other) 
        :mystd::Vector<char> (1, other)
    {}

    String& String::operator=(const char* other) {
        Resize(strlen(other));
        for (size_t index = 0; index < Size(); ++index) {
            operator[](index) = other[index];
        }
        return *this;
    }

    String& String::operator=(const char other) {
        Resize(1);
        operator[](0) = other;
        return *this;
    }

    void String::Upper(const size_t begin, const size_t end) {
        if (end <= begin || end > Size()) throw ex1;
        const int8_t change = 'A' - 'a';
        for (size_t index = begin; index < end; ++index) {
            char &data = operator[](index);
            if (data >= 'a' && data <= 'z') {
                data += change;
            } 
        }
    }

    void String::Lower(const size_t begin, const size_t end) {
        if (end <= begin || end > Size()) throw ex1;
        const int8_t change = 'a' - 'A';
        for (size_t index = begin; index < end; ++index) {
            char &data = operator[](index);
            if (data >= 'A' && data <= 'Z') {
                data += change;
            } 
        }
    }

    void String::Upper() {
        const int8_t change = 'A' - 'a';
        for (size_t index = 0; index < Size(); ++index) {
            char &data = operator[](index);
            if (data >= 'a' && data <= 'z') {
                data += change;
            } 
        }
    }

    void String::Lower() {
        const int8_t change = 'a' - 'A';
        for (size_t index = 0; index < Size(); ++index) {
            char &data = operator[](index);
            if (data >= 'A' && data <= 'Z') {
                data += change;
            } 
        }
    }

    int64_t String::ToInt64() {
        size_t index = 0; 
        while(operator[](index) == ' ') ++index;
        bool sign = false;
        if (operator[](index) == '-') {
            ++index;
            sign = true;
        }
        if (Size() - index > 19) {
            throw ex2;
        }
        uint64_t value = 0;
        while (index < Size()) {
            if (operator[](index) < '0' || '9' > operator[](index)) {
                throw ex2;
            }
            value *= 10;
            value += (operator[](index) - '0');
            ++index;
        }
        if (value > INT64_MAX) {
            throw ex2;
        }
        int64_t answer = static_cast<int64_t>(value);
        if (sign) answer *= -1;
        return answer;
    }

}