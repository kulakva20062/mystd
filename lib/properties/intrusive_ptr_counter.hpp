#pragma once
#include <atomic>
#include <cstddef>
#include <type_traits>

namespace mystd {

    template<typename T>
    requires std::is_class_v<T>
    class IntrusivePtrCounter {
        public: 
            inline static size_t GetCount(T* p) {
                return p->count.load();
            }

            inline static void intrusive_ptr_add_ref(T* p) noexcept {
                ++(p->count);
            }

            inline static void intrusive_ptr_release(T* p) noexcept {
                if ((--(p->count)) == 0)
                    delete p;
            }

        protected:
            IntrusivePtrCounter() = default;

            ~IntrusivePtrCounter() = default;

        private:
            std::atomic<size_t> count = 0;

    };

} // namespace mystd