#pragma once
#include <cstddef>

namespace mystd {

    template<typename T>
    class PoolAllocator {
        public:
            using value_type = T;

            PoolAllocator() {
                pool = new T[1000];
            }

            ~PoolAllocator() {
                delete[] pool;
            }

            T* allocate(size_t size) {
                return new T[size];
            }
            void deallocate(T* ptr, size_t size) {
                delete[] ptr;
            }
            
            
        private:
            T* pool;
    };

} // namespace mystd