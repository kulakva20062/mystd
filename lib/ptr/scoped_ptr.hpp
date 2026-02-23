#pragma once

namespace mystd {

    template<typename T>
    class ScopedPtr {
        public:
            explicit ScopedPtr(T* ptr)  
                :ptr_(ptr)
            {}

            ScopedPtr(const ScopedPtr& other) = delete;

            ScopedPtr& operator=(const ScopedPtr& other) = delete;

            ~ScopedPtr() {
                delete ptr_;
            }

            T* operator->() const {
                return ptr_;
            }

            T& operator*() const {
                return *ptr_;
            }

            void Reset(T* ptr) {
                delete ptr_;
                ptr_ = ptr;
            }

            T* Get() const {
                return ptr_;
            }

        private:
            T* ptr_;
    };

}