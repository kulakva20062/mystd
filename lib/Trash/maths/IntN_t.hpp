#pragma once
#include <concepts>
#include <cstddef>
#include <climits>
#include <algorithm>
#include "../../bit.hpp"
#include <cstring>
#include <cstdint>
#include <iostream>
#include "../lib/exception.hpp"

namespace mystd {


    template<size_t width>
    requires (width >= 8)
    class IntN_t {
        public:
            template<typename T>
            requires (std::is_integral_v<T> && sizeof(T) * CHAR_BIT <= width)
            IntN_t(T value = static_cast<T>(0)) 
                :data_(new std::byte[(width + CHAR_BIT - 1) / CHAR_BIT])
            {
                std::copy(reinterpret_cast<std::byte*>(&value), reinterpret_cast<std::byte*>(&value) + sizeof(T), data_);
            }

            ~IntN_t() {
                delete[] data_;
            }

            IntN_t(const IntN_t<width>& other)
                :data_(new std::byte[other.SizeBytes()])
            {
                std::copy(other.Data(), other.Data() + other.SizeBytes(), data_);
            }
            
            template<typename T>
            requires (width % CHAR_BIT == 0, sizeof(T) == CHAR_BIT)
            explicit IntN_t(const T* data)
                : data_(new std::byte[(width + CHAR_BIT - 1) / CHAR_BIT])
            {
                std::copy(data, data + SizeBytes(), data_);
            }

            template<size_t other_width>
            requires (other_width < width)
            IntN_t(const IntN_t<other_width>& other)
                :data_(new std::byte[(width + CHAR_BIT - 1) / CHAR_BIT])
            {
                std::copy(other.Data(), other.Data() + other.SizeBytes(), data_);
                const bool bit = mystd::GetBit(data_, other_width - 1);
                SetLineBits(data_, other_width, width, bit);
            }

            template<size_t other_width>
            requires (other_width > width)
            explicit IntN_t(const IntN_t<other_width>& other)
                :data_(new std::byte[(width + CHAR_BIT - 1) / CHAR_BIT])
            {
                std::copy(other.Data(), other.Data() + SizeBytes(), data_);
            }

            IntN_t(IntN_t&& other)
                :data_(other.Data())
            {
                other.DataPtr() = nullptr;
            }

            IntN_t<width>& operator=(const IntN_t<width>& other) {
                if(this == &other) {
                    return *this;
                }
                std::copy(other.Data(), other.Data() + other.SizeBytes(), data_);
                return *this;
            }

            template<size_t other_width>
            requires (other_width > width)
            IntN_t<width>& operator=(const IntN_t<other_width>& other) {
                std::copy(other.Data(), other.Data() + SizeBytes(), data_);
            }

            IntN_t<width>& operator=(IntN_t<width>&& other) {
                if(this == &other) {
                    return *this;
                }
                data_ = other.Data();
                other.DataPtr() = nullptr;
                return *this;
            }

            bool operator==(const IntN_t<width>& other) const {
                return std::memcmp(data_, other.Data(), SizeBytes()) == 0;
            }

            std::strong_ordering operator<=>(const IntN_t<width>& other) const {
                for (size_t positions = SizeBytes(); positions > 0; --positions) {
                    size_t position = positions - 1;
                    const std::byte a = reinterpret_cast<const std::byte&>(data_[position]);
                    const std::byte b = reinterpret_cast<const std::byte&>(other.Data()[position]);
                    if (a < b) return std::strong_ordering::less;
                    if (a > b) return std::strong_ordering::greater;
                }
                return std::strong_ordering::equal;
            }

            //FIXME: переписать
            IntN_t<width>& operator>>=(const size_t& shift) {
                if (shift >= width) {
                    const bool sign_bit = mystd::GetBit(data_, width - 1);
                    mystd::SetLineBits(data_, 0, width, sign_bit);
                    return *this;
                }
                if (shift == 0) {
                    return *this;
                }
                for (size_t index = 0 + shift; index < width; ++index) {
                    mystd::SetBit(data_, index - shift, mystd::GetBit(data_, index));
                }
                mystd::SetLineBits(data_, width - shift, width, mystd::GetBit(data_, width - 1));
                return *this;
            }

            IntN_t<width> operator>>(const size_t& shift) const {
                IntN_t<width> result(*this);
                result >>= shift;
                return result;
            }

            //FIXME: переписать
            IntN_t<width>& operator<<=(const size_t& shift) {
                if (shift >= width) {
                    mystd::SetLineBits(data_, 0, width, false);
                    return *this;
                }
                if (shift == 0) {
                    return *this;
                }     
                size_t position;
                for (size_t index = width - shift; index > 0; --index) {
                    position = index - 1;
                    mystd::SetBit(data_, position + shift, mystd::GetBit(data_, position));
                }
                mystd::SetLineBits(data_, 0, shift, false);
                return *this;
            }

            IntN_t<width> operator<<(const size_t& shift) const {
                IntN_t<width> result(*this);
                result <<= shift;
                return result;
            }

            IntN_t<width>& operator+=(const IntN_t<width>& other) {
                bool overflow = false;
                uint_least8_t carry;
                for (size_t index = 0; index < SizeBytes(); ++index) {
                    carry = reinterpret_cast<uint_least8_t&>(data_[index]) + reinterpret_cast<uint_least8_t&>(other.Data()[index]);
                    if (overflow) {
                        ++carry;
                        overflow = false;
                    }
                    if (carry < reinterpret_cast<uint_least8_t&>(data_[index])) {
                        overflow = true;
                    }
                    data_[index] = reinterpret_cast<std::byte&>(carry);
                }
                return *this;
            }

            friend IntN_t<width> operator+(const IntN_t<width>& lhs, const IntN_t<width>& rhs) {
                IntN_t<width> result(lhs);
                result += rhs;
                return result;
            }

            IntN_t<width>& operator-=(const IntN_t<width>& other) {
                bool overflow = false;
                uint_least8_t carry;
                for (size_t index = 0; index < SizeBytes(); ++index) {
                    carry = reinterpret_cast<uint_least8_t&>(data_[index]) - reinterpret_cast<uint_least8_t&>(other.Data()[index]);
                    if (overflow) {
                        --carry;
                        overflow = false;
                    }
                    if (carry > reinterpret_cast<uint_least8_t&>(data_[index])) {
                        overflow = true;
                    }
                    data_[index] = reinterpret_cast<std::byte&>(carry);
                }
                return *this;
            }

            friend IntN_t<width> operator-(const IntN_t<width>& lhs, const IntN_t<width>& rhs) {
                IntN_t<width> result(lhs);
                result -= rhs;
                return result;
            }
            
            IntN_t<width>& operator*=(const IntN_t<width>& other) {
                uint_least8_t overflow = 0;
                short carry;
                for (size_t index = 0; index < SizeBytes(); ++index) {
                    carry = static_cast<short>(reinterpret_cast<uint_least8_t&>(data_[index])) * static_cast<short>(reinterpret_cast<uint_least8_t&>(other.Data()[index]));
                    carry += overflow;
                    overflow = reinterpret_cast<uint_least8_t&>(carry) >> 8;
                    data_[index] = reinterpret_cast<std::byte&>(carry);
                }
                return *this;
            }

            friend IntN_t<width> operator*(const IntN_t<width>& lhs, const IntN_t<width>& rhs) {
                IntN_t<width> result(lhs);
                result *= rhs;
                return result;
            }

            IntN_t<width>& operator/=(const IntN_t<width>& other) {
                if (other == IntN_t<width>(0)) { throw mystd::Exception("Division by zero", 1); }
                IntN_t<width> dividend(*this);
                IntN_t<width> divisor(other);
                IntN_t<width> quotient(0);
                while (dividend >= divisor) {
                    dividend -= divisor;
                    quotient += 1;
                }
                *this = quotient;
                return *this;
            }
            
            friend IntN_t<width> operator/(const IntN_t<width>& lhs, const IntN_t<width>& rhs) {
                IntN_t<width> result(lhs);
                result /= rhs;
                return result;
            }

            IntN_t<width>& operator%=(const IntN_t<width>& other) {
                if (other == IntN_t<width>(0)) { throw mystd::Exception("Division by zero", 1); }
                IntN_t<width> dividend(*this);
                IntN_t<width> divisor(other);
                while (dividend >= divisor) {
                    dividend -= divisor;
                }
                *this = dividend;
                return *this;
            }

            friend IntN_t<width> operator%(const IntN_t<width>& lhs, const IntN_t<width>& rhs) {
                IntN_t<width> result(lhs);
                result %= rhs;
                return result;
            }

            friend std::ostream& operator<<(std::ostream& os, const IntN_t<width>& val) {
                IntN_t<width> value(val);
                while (value != static_cast<IntN_t<width>>(static_cast<int8_t>(0))) {
                    os << int(static_cast<IntN_t<8>>(value % static_cast<IntN_t<width>>(static_cast<int8_t>(10))).ToInt<int8_t>());
                    value /= static_cast<IntN_t<width>>(static_cast<int8_t>(10));
                }
                return os;
            }

            inline size_t Size() const {
                return width;
            }

            inline size_t SizeBytes() const {
                return (width + CHAR_BIT - 1) / CHAR_BIT;
            }

            inline std::byte* Data() const {
                return data_;
            }

            inline std::byte*& DataPtr() {
                return data_;
            }

            template<typename T>
            requires (std::is_integral_v<T> && std::signed_integral<T> && sizeof(T) * CHAR_BIT == width) 
            T ToInt() const {
                return reinterpret_cast<T&>(data_[0]);
            }

        private:
            std::byte* data_;

    };

}