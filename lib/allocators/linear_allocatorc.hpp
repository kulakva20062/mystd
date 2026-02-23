#pragma once
#include <type_traits>
#include <vector>

namespace mystd {

    template<typename T, size_t size = 1024, size_t initial_blocks = 1>
    requires (size > 0)
    class LinearAllocator {
        public:
            using value_type = T;

            template<typename U>
            struct rebind {
                using other = LinearAllocator<U, size, initial_blocks>;
            };

            LinearAllocator() {
                data_.reserve(initial_blocks);
                data_.push_back(new T[size]);
            }

            ~LinearAllocator() {
                for (auto& block : data_) {
                    delete[] block;
                }
            }

            T* allocate(size_t n) {
                if (n > size) {
                    return new T[n];
                }

                if (current_block_index_ + n > size) {
                    current_block_index_ = 0;
                    data_.push_back(new T[size]);
                }

                current_block_index_ += n;
                return data_.back() + current_block_index_ - n;
            }

            void deallocate(T* ptr, size_t n) {
                if (n > size) {
                    delete[] ptr;
                    return;
                }
            }

            void reset(size_t new_size = size) {
                for (auto& block : data_) {
                    delete[] block;
                }
                data_.clear();
                data_.push_back(new T[new_size]);
                current_block_index_ = 0;
            }

            inline void clear() {
                reset();
            }

            template<typename U>
            bool operator==(const LinearAllocator<U, size>& other) const noexcept {
                if constexpr (std::is_same_v<T, U>) {
                    return this == &other;
                }
                return false;
            }

            template<typename U>
            bool operator!=(const LinearAllocator<U, size>& other) const noexcept {
                return !(*this == other); // Реализован через оператор ==
            }

        private:
            std::vector<T*> data_;

            size_t current_block_index_ = 0;
    };

} //namespace mystd