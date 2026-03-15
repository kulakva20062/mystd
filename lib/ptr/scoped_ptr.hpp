#pragma once

namespace mystd {

    template<typename T>
    class IntrusivePtr {
        public:
            explicit IntrusivePtr(T* ptr)  
                :ptr_(ptr)
            {}

            IntrusivePtr(const IntrusivePtr& other) = delete;

            IntrusivePtr& operator=(const IntrusivePtr& other) = delete;

            ~IntrusivePtr() {
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

} // namespace mystd