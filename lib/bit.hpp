#pragma once
#include <cstddef>
#include <climits>
#include <type_traits>

namespace {

    template<typename T>
    concept NotPointer = !std::is_pointer_v<T>;

}

namespace mystd {

    #if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    inline bool GetBit(const void* value, size_t index) {
        const std::byte mask = static_cast<std::byte>(1) << (index % CHAR_BIT);
        return static_cast<bool>(*(reinterpret_cast<const std::byte*>(value) + (index / CHAR_BIT)) & mask);
    }

    inline void SetBit(void* value, size_t index, bool bit) {
        const std::byte mask = static_cast<std::byte>(1) << (index % CHAR_BIT);
        bit ? *(reinterpret_cast<std::byte*>(value) + (index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (index / CHAR_BIT)) &= ~mask;
    }

    template<NotPointer T>
    inline T SetBit(T value, size_t index, bool bit) {
        SetBit(&value, index, bit);
        return value;
    }

    inline void ToggleBit(void* value, size_t index) {
        const std::byte mask = static_cast<std::byte>(1) << (index % CHAR_BIT);
        *(reinterpret_cast<std::byte*>(value) + (index / CHAR_BIT)) ^= mask;
    }

    template<NotPointer T>
    inline T ToggleBit(T value, size_t index) {
        ToggleBit(&value, index);
        return value;
    }

    //[start_index, end_index)
    inline void SetLineBits(void* value, size_t start_index, size_t end_index, bool bit) {
        for (std::byte* ptr = reinterpret_cast<std::byte*>(value) + (start_index + CHAR_BIT - 1) / CHAR_BIT;
             ptr < reinterpret_cast<std::byte*>(value) + end_index / CHAR_BIT; ++ptr) {
            *ptr = bit ? (~static_cast<std::byte>(0)) : static_cast<std::byte>(0);
        }
        if (start_index / CHAR_BIT != end_index / CHAR_BIT) {
            if (start_index % CHAR_BIT != 0) {
                const std::byte mask = (~static_cast<std::byte>(0)) << (start_index % CHAR_BIT);
                bit ? *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) &= ~mask;
            }
            if (end_index % CHAR_BIT != 0) {
                const std::byte mask = (~static_cast<std::byte>(0)) >> (CHAR_BIT - (end_index % CHAR_BIT));
                bit ? *(reinterpret_cast<std::byte*>(value) + (end_index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (end_index / CHAR_BIT)) &= ~mask;
            }
        } else {
            const std::byte mask = (~static_cast<std::byte>(0)) >> (CHAR_BIT - (end_index % CHAR_BIT)) & (~static_cast<std::byte>(0)) << (start_index % CHAR_BIT);
            bit ? *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) &= ~mask;
        }
    }

    template<NotPointer T>
    inline T SetLineBits(T value, size_t start_index, size_t end_index, bool bit) {
        SetLineBits(&value, start_index, end_index, bit);
        return value;
    }

    //[first, last)
    //[start_index, end_index)
    inline void Mask(void* first, void* last, size_t start_index, size_t end_index) {
        SetLineBits(first, 0, start_index, false);
        SetLineBits(first, end_index, (reinterpret_cast<std::byte*>(last) - reinterpret_cast<std::byte*>(first)) * CHAR_BIT, false);
    }

    //[start_index, end_index)
    template<NotPointer T>
    inline T Mask(T value, size_t start_index, size_t end_index) {
        SetLineBits(&value, 0, start_index, false);
        SetLineBits(&value, end_index, sizeof(T) * CHAR_BIT, false);
        return value;
    }

    #elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

    inline bool GetBit(const void* value, size_t index) {
        const std::byte mask = static_cast<std::byte>(1) << (CHAR_BIT - 1 - (index % CHAR_BIT));
        return static_cast<bool>(*(reinterpret_cast<const std::byte*>(value) + (index / CHAR_BIT)) & mask);
    }

    inline void SetBit(void* value, size_t index, bool bit) {
        const std::byte mask = static_cast<std::byte>(1) << (CHAR_BIT - 1 - (index % CHAR_BIT));
        bit ? *(reinterpret_cast<std::byte*>(value) + (index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (index / CHAR_BIT)) &= ~mask;
    }

    template<NotPointer T>
    inline T SetBit(T value, size_t index, bool bit) {
        SetBit(&value, index, bit);
        return value;
    }

    inline void ToggleBit(void* value, size_t index) {
        const std::byte mask = static_cast<std::byte>(1) << (CHAR_BIT - 1 - (index % CHAR_BIT));
        *(reinterpret_cast<std::byte*>(value) + (index / CHAR_BIT)) ^= mask;
    }

    template<NotPointer T>
    inline T ToggleBit(T value, size_t index) {
        ToggleBit(&value, index);
        return value;
    }

    //[start_index, end_index)
    inline void SetLineBits(void* value, size_t start_index, size_t end_index, bool bit) {
        for (std::byte* ptr = reinterpret_cast<std::byte*>(value) + (start_index + CHAR_BIT - 1) / CHAR_BIT;
             ptr < reinterpret_cast<std::byte*>(value) + end_index / CHAR_BIT; ++ptr) {
            *ptr = bit ? (~static_cast<std::byte>(0)) : static_cast<std::byte>(0);
        }
        if (start_index / CHAR_BIT != end_index / CHAR_BIT) {
            if (start_index % CHAR_BIT != 0) {
                const std::byte mask = (~static_cast<std::byte>(0)) >> (start_index % CHAR_BIT);
                bit ? *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) &= ~mask;
            }
            if (end_index % CHAR_BIT != 0) {
                const std::byte mask = (~static_cast<std::byte>(0)) << (CHAR_BIT - (end_index % CHAR_BIT));
                bit ? *(reinterpret_cast<std::byte*>(value) + (end_index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (end_index / CHAR_BIT)) &= ~mask;
            }
        } else {
            const std::byte mask = (~static_cast<std::byte>(0)) >> (start_index % CHAR_BIT) & (~static_cast<std::byte>(0)) << (CHAR_BIT - (end_index % CHAR_BIT));
            bit ? *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) |= mask : *(reinterpret_cast<std::byte*>(value) + (start_index / CHAR_BIT)) &= ~mask;
        }
    }

    template<NotPointer T>
    inline T SetLineBits(T value, size_t start_index, size_t end_index, bool bit) {
        SetLineBits(&value, start_index, end_index, bit);
        return value;
    }

    //[first, last)
    //[start_index, end_index)
    inline void Mask(void* first, void* last, size_t start_index, size_t end_index) {
        SetLineBits(first, 0, start_index, false);
        SetLineBits(first, end_index, (reinterpret_cast<std::byte*>(last) - reinterpret_cast<std::byte*>(first)) * CHAR_BIT, false);
    }

    //[start_index, end_index)
    template<NotPointer T>
    inline T Mask(T value, size_t start_index, size_t end_index) {
        SetLineBits(&value, 0, start_index, false);
        SetLineBits(&value, end_index, sizeof(T) * CHAR_BIT, false);
        return value;
    }

    #else
        #error "Unsupported byte order (mixed endian)"

    #endif

} // namespace mystd