#pragma once
#include <type_traits>
#include <atomic>

namespace mystd {

    template<typename T>
    requires std::is_class_v<T>
    class Counter {
        public: 
            const static size_t GetCount() {
                return count.load();
            }

        protected:
            Counter() {
                ++count;
            }

            ~Counter() {
                --count;
            }

        private:
            inline static std::atomic<size_t> count = 0;
            
    };

}