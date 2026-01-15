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
    
    std::ostream& operator<<(std::ostream& os, const String& string) {
        os.write(string.Data(), string.Size());
        return os;
    }
    
    //Возможно можно(возможно) реализовать через буфер, но я не приудумал как.
    std::istream& operator>>(std::istream& is, String& string) {
        string.Resize(0);
        char c;
        while (is.get(c)) {
            if (c != ' ' && c != '\n') {
                string.PushBack(c);
                break;
            }
        }
        while (is.get(c)) {
            if (c == ' ') {
                return is;
            }
            string.PushBack(c);
            if (c == '\n') {
                return is;
            }
        }
        return is;
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
    
    String String::SubStr(size_t begin, size_t end) const {
        if (begin > end) {
            throw Exception("Неверный диапазон, SubStr", 2);
        } 
        if (Size() < end) {
            end = Size();
            if (end < begin) {
                begin = end;
            }
        }
        String answer = *this;
        answer.Resize(end - begin);
        for (size_t index = begin; index < end; ++index) {
            answer[index - begin] = operator[](index);
        }
        return answer;
    }
    
    const char* String::CStr() const {
        return Data();
    }
    
    //FIXME:
    //Сделать проверку на переполнение
    int64_t String::ToInt() const {
        int64_t answer = 0;
        int8_t sign = 1;
        size_t index = 0;
        if (operator[](0) == '-') {
            sign = -1;
            ++index;
        }
        for (; index < Size(); ++index) {
            if (operator[](index) >= '0' && operator[](index) <= '9') {
                answer = answer * 10 + operator[](index) - '0';
            } else {
                throw Exception("Неверный формат числа, ToInt()", 2);
            }
        }
        return answer * sign;
    }
    
    long double String::ToDouble() const {
        long double answer = 0;
        int8_t sign = 1;
        size_t index = 0;
        if (operator[](0) == '-') {
            sign = -1;
            ++index;
        }
        for (; index < Size(); ++index) {
            if (operator[](index) >= '0' && operator[](index) <= '9') {
                answer = answer * 10 + operator[](index) - '0';
            } else if (operator[](index) == '.') {
                ++index;
                size_t step = 10;
                for (; index < Size(); ++index) {
                    if (operator[](index) >= '0' && operator[](index) <= '9') {
                        answer += static_cast<long double>(operator[](index) - '0') / step;
                        step *= 10;
                    } else {
                        throw Exception("Неверный формат числа, ToDouble()", 2);
                    }
                }
            } else {
                throw Exception("Неверный формат числа, ToDouble()", 2);
            }
        }
        return answer * sign;
    }
    
}