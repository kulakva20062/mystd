#pragma once
#include <climits>
#include <cstring>
#include <cstddef>
#include <concepts>
#include <compare>
#include <cstdint>
#include <bit>

namespace mystd {

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define MYSTD_LITTLE_ENDIAN
    #elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define MYSTD_BIG_ENDIAN
    #else
        #error "Error: не понятно, в каком порядке байты (mystd int.hpp)"
    #endif

    template<size_t width>
    requires (width > 0 && width <= SIZE_MAX)
    class UIntN_t final {
        public:
            template<typename T>
            requires (std::integral<T> && std::unsigned_integral<T> && (sizeof(T) <= width / CHAR_BIT))
            UIntN_t(T value = 0) 
                :data_(new std::byte[SizeBytes()])
            {
                size_t size_bytes = sizeof(T);
                #if defined(MYSTD_LITTLE_ENDIAN)
                    std::memcpy(data_, &value, size_bytes);
                #elif defined(MYSTD_BIG_ENDIAN)
                    std::memcpy(data_ + (SizeBytes() - size_bytes), &value, size_bytes);
                #endif
            }

            ~UIntN_t() {
                delete[] data_;
            }

            UIntN_t(const UIntN_t<width>& other) 
                :data_(new std::byte[SizeBytes()])
            {
                std::memcpy(data_, other.Data(), SizeBytes());
            }

            template<size_t other_width>
            requires (other_width < width)
            UIntN_t(const UIntN_t<other_width>& other) 
                :data_(new std::byte[SizeBytes()])
            {
                #if defined(MYSTD_LITTLE_ENDIAN)
                    std::memset(data_ + other.SizeBytes(), 0, SizeBytes() - other.SizeBytes());
                    std::memcpy(data_, other.Data(), other.SizeBytes());
                #elif defined(MYSTD_BIG_ENDIAN)
                    std::memset(data_, 0, SizeBytes() - other.SizeBytes());
                    std::memcpy(data_ + (SizeBytes() - other.SizeBytes()), other.Data(), other.SizeBytes());
                #endif
            } 

            template<size_t other_width>
            requires (other_width > width)
            explicit UIntN_t(const UIntN_t<other_width>& other) 
                :data_(new std::byte[SizeBytes()])
            {
                #if defined(MYSTD_LITTLE_ENDIAN)
                    std::memcpy(data_, other.Data(), SizeBytes());
                #elif defined(MYSTD_BIG_ENDIAN)
                    std::memcpy(data_, other.Data() + (other.SizeBytes() - SizeBytes()), SizeBytes());   
                #endif
            }

            UIntN_t(UIntN_t<width>&& other)
                :data_(other.Data())
            {
                other.DataPointer() = nullptr;
            }

            UIntN_t& operator=(const UIntN_t<width>& other) {
                if (this == &other) {
                    return *this;
                }
                std::memcpy(data_, other.Data(), SizeBytes());
                return *this;
            }

            template<size_t other_width>
            requires (other_width > width)
            UIntN_t& operator=(const UIntN_t<other_width>& other) {
                if (this == &other) {
                    return *this;
                }
                #if defined(MYSTD_LITTLE_ENDIAN)
                    std::memcpy(data_, other.Data(), SizeBytes());
                #elif defined(MYSTD_BIG_ENDIAN)
                    std::memcpy(data_, other.Data() + (other.SizeBytes() - SizeBytes()), SizeBytes());
                #endif
                return *this;
            }

            UIntN_t& operator=(UIntN_t<width>&& other) {
                if (this == &other) {
                    return *this;
                }
                data_ = other.Data();
                other.Data() = nullptr;
                return *this;
            }

            bool operator==(const UIntN_t<width>& other) const {
                return std::memcmp(data_, other.Data(), SizeBytes()) == 0;
            }

            //Чтобы определить все знаки сравнения сразу.
            std::strong_ordering operator<=>(const UIntN_t<width>& other) const {
                #if defined(MYSTD_LITTLE_ENDIAN)
                    for (size_t positions = SizeBytes(); positions > 0; --positions) {
                        const std::byte a = reinterpret_cast<const std::byte&>(data_[positions - 1]);
                        const std::byte b = reinterpret_cast<const std::byte&>(other.Data()[positions - 1]);
                        if (a < b) return std::strong_ordering::less;
                        if (a > b) return std::strong_ordering::greater;
                    }
                    return std::strong_ordering::equal;
                #elif defined(MYSTD_BIG_ENDIAN)
                    int cmp = std::memcmp(data_, other.Data(), SizeBytes());
                    if (cmp < 0) return std::strong_ordering::less;
                    if (cmp > 0) return std::strong_ordering::greater;
                    return std::strong_ordering::equal;
                #endif
            }

            // //При Биг эндиан всё остаётся также?
            UIntN_t<width> operator>>(const size_t& shift) {
                UIntN_t<width> result(*this);
                std::memmove(result.Data(), result.Data() + shift, SizeBytes() - shift);
                return result;
            }

            UIntN_t<width> operator<<(const size_t& shift) {
                UIntN_t<width> result(*this);
                std::memmove(result.Data() + shift, result.Data(), SizeBytes() - shift);
                return result;
            }
            
            friend UIntN_t<width>& operator+=(UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                bool overflow = false;
                #if UINTMAX_WIDTH % 2 == 0
                    alignas(UINTMAX_WIDTH) uint_least8_t result = 0;
                #else
                    uint_least8_t result = 0;
                #endif
    
                #if defined(MYSTD_LITTLE_ENDIAN)
                    for (size_t position = 0; position < lhs.SizeBytes(); ++position) {
                        result = reinterpret_cast<uint_least8_t&>(lhs.data_[position]);
                        result += reinterpret_cast<uint_least8_t&>(rhs.data_[position]);
                        if (overflow) {
                            result += 1;
                            overflow = false;
                        }
                        if (result < reinterpret_cast<uint_least8_t&>(lhs.data_[position])) {
                            overflow = true;
                        }
                    }
                #elif defined(MYSTD_BIG_ENDIAN)
                    for (size_t position = lhs.SizeBytes(); position > 0; --position) {
                        size_t position_ = position - 1;
                        result = reinterpret_cast<uint_least8_t&>(lhs.data_[position_]);
                        result += reinterpret_cast<uint_least8_t&>(rhs.data_[position_]);
                        if (overflow) {
                            result += 1;
                            overflow = false;
                        }
                        if (result < reinterpret_cast<uint_least8_t&>(lhs.data_[position_])) {
                            overflow = true;
                        }
                    }
                #endif

                return lhs;
            }

            friend UIntN_t<width>& operator-=(UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                bool overflow = false;
                #if UINTMAX_WIDTH % 2 == 0
                    alignas(UINTMAX_WIDTH) uint_least8_t result = 0;
                #else
                    uint_least8_t result = 0;
                #endif

                #if defined(MYSTD_LITTLE_ENDIAN)
                    for (size_t position = 0; position < lhs.SizeBytes(); ++position) {
                        result = reinterpret_cast<uint_least8_t&>(lhs.data_[position]);
                        result -= reinterpret_cast<uint_least8_t&>(rhs.data_[position]);
                        if (overflow) {
                            result -= 1;
                            overflow = false;
                        }
                        if (result > reinterpret_cast<uint_least8_t&>(lhs.data_[position])) {
                            overflow = true;
                        }
                    }
                #elif defined(MYSTD_BIG_ENDIAN)
                    for (size_t position = lhs.SizeBytes(); position > 0; --position) {
                        size_t position_ = position - 1;
                        result = reinterpret_cast<uint_least8_t&>(lhs.data_[position_]);
                        result -= reinterpret_cast<uint_least8_t&>(rhs.data_[position_]);
                        if (overflow) {
                            result -= 1;
                            overflow = false;
                        }
                        if (result > reinterpret_cast<uint_least8_t&>(lhs.data_[position])) {
                            overflow = true;
                        }
                    }
                #endif
                return lhs;
            }

            friend UIntN_t<width>& operator*=(UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                constexpr size_t result_width = std::bit_ceil(width);
                //Да долго, но ничего не могу придумать лучше.
                //Пишу его в первый раз, потом возможно оптимизирую.
                UIntN_t<result_width> result(0U);
                UIntN_t<result_width> lhs_copy(lhs);
                UIntN_t<result_width> rhs_copy(rhs);

                #if defined(MYSTD_LITTLE_ENDIAN)
                    UIntN_t<result_width / 2> lhs_part_left = static_cast<UIntN_t<result_width / 2>>(lhs_copy);
                    UIntN_t<result_width / 2> lhs_part_right = static_cast<UIntN_t<result_width / 2>>(lhs_copy >> (result_width / 2));
                    UIntN_t<result_width / 2> rhs_part_left = static_cast<UIntN_t<result_width / 2>>(rhs_copy);
                    UIntN_t<result_width / 2> rhs_part_right = static_cast<UIntN_t<result_width / 2>>(rhs_copy >> (result_width / 2));

                    UIntN_t<result_width> z0 = lhs_part_left * rhs_part_left;
                    UIntN_t<result_width> z1 = (lhs_part_left + lhs_part_right) * (rhs_part_left + rhs_part_right);
                    UIntN_t<result_width> z2 = lhs_part_right * rhs_part_right;

                    result = z0 + (z1 - z0 - z2) << (result_width / 2) + z2;
                #endif
                lhs = result;
                return lhs;
            }

            friend UIntN_t<width>& operator/=(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                bool overflow = false;

                #if defined(MYSTD_LITTLE_ENDIAN)
                #endif
                return lhs;
            }

            friend UIntN_t<width>& operator%=(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                bool overflow = false;

                #if defined(MYSTD_LITTLE_ENDIAN)
                #endif
                return lhs;
            }

            friend UIntN_t<width> operator+(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                return UIntN_t<width>(lhs) += rhs;
            }

            friend UIntN_t<width> operator-(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                return UIntN_t<width>(lhs) -= rhs;
            }

            friend UIntN_t<width> operator*(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                return UIntN_t<width>(lhs) *= rhs;
            }

            friend UIntN_t<width> operator/(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                return UIntN_t<width>(lhs) /= rhs;
            }

            friend UIntN_t<width> operator%(const UIntN_t<width>& lhs, const UIntN_t<width>& rhs) {
                return UIntN_t<width>(lhs) %= rhs;
            }

            static inline size_t Size() {
                return width;
            }

            static inline size_t SizeBytes() {
                return (width + CHAR_BIT - 1) / CHAR_BIT;
            }

            std::byte*& DataPointer() {
                return data_;
            }

            std::byte* Data() {
                return data_;
            }

            const std::byte* Data() const {
                return data_;
            }

        private:
            std::byte* data_;
    };

    // template<size_t width>
    // std::ostream& operator<<(std::ostream& os, const UIntN_t<width>& value) {
    //     std::string result;
    //     while(value != 0) {
    //         result.push_back(value / 10);
    //         value %= 10;
    //     }
    //     return os;
    // }

    #if defined(MYSTD_LITTLE_ENDIAN) 
        #undef MYSTD_LITTLE_ENDIAN
    #elif defined(MYSTD_BIG_ENDIAN)
        #undef MYSTD_BIG_ENDIAN
    #endif

} //namespace mystd