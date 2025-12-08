#include "string.hpp"
#include "vector.hpp"
#include "exception.hpp"
#include <cstring>
#include <cstdint>

namespace mystd {

    String::String(size_t size, char default_value) : mystd::Vector<char> (size, default_value) {}

    String::~String() {}

    String::String(const String& other) : mystd::Vector<char> (other) {}
    
    String::String(const char* other) {
        if (other == nullptr) {
            Resize(0);
            return;
        }
        size_t len = strlen(other);
        Resize(len);
        for (size_t index = 0; index < len; ++index) {
            operator[](index) = other[index];
        }
    }
    
    String::String(char other) {
        Resize(1);
        operator[](0) = other;
    }

    String& String::operator=(const String& other) {
        if (this == &other) return *this;
        Resize(other.Size());
        for (size_t index = 0; index < Size(); ++index) {
            operator[](index) = other[index];
        }
        return *this;
    }

    bool operator==(const String& lhs, const String& rhs) {
        if (lhs.Size() != rhs.Size()) return false;
        for (size_t index = 0; index < lhs.Size(); ++index) {
            if (lhs[index] != rhs[index]) return false;
        }
        return true;
    }

    bool operator!=(const String& lhs, const String& rhs) {
        return !(rhs == lhs);
    }

    String& String::operator+=(const String& other) {
        size_t size = Size();
        size_t other_size = other.Size();
        Resize(size + other_size);
        for (size_t index = 0; index < other_size; ++index) {
            operator[](size + index) = other[index];
        }
        return *this;
    }

    String String::operator+(const String& other) const {
        String result = *this;
        result += other;
        return result;
    }

    void String::Upper(const size_t begin, const size_t end) {
        if (end <= begin || end > Size()) return;
        const int8_t change = 'A' - 'a';
        for (size_t index = begin; index < end; ++index) {
            char &data = operator[](index);
            if (data >= 'a' && data <= 'z') {
                data += change;
            } 
        }
    }

    void String::Lower(const size_t begin, const size_t end) {
        if (end <= begin || end > Size()) return;
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
            throw Exception("Перевод из строки в число невозможен", 2);
        }
        uint64_t value = 0;
        while (index < Size()) {
            if (operator[](index) < '0' || '9' < operator[](index)) {
                throw Exception("Перевод из строки в число невозможен", 2);
            }
            value *= 10;
            value += (operator[](index) - '0');
            ++index;
        }
        if (value > INT64_MAX) {
            throw Exception("Перевод из строки в число невозможен", 2);
        }
        int64_t answer = static_cast<int64_t>(value);
        if (sign) answer *= -1;
        return answer;
    }
    
    String String::SubStr(const size_t begin, const size_t end) const {
        if (begin > end || end > Size()) {
            throw Exception("Неверный диапазон", 2);
        }
        String answer = *this;
        answer.Resize(end - begin + 1);
        for (size_t index = begin; index <= end; ++index) {
            answer[index - begin] = operator[](index);
        }
        return answer;
    }

}