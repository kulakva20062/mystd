#pragma once
#include "vector.hpp"

namespace mystd {

    class String : public Vector<char> {
        public:
            explicit String(size_t size = 0, char default_value = '\0');

            ~String() override;

            String(const String& other);

            String(const char* other);

            String(const char other);

            String& operator=(const char* other);

            String& operator=(const char other);

            void Upper(const size_t begin, const size_t end);

            void Lower(const size_t begin, const size_t end);

            void Upper();

            void Lower();

    };

}  // namespace mystd
