#pragma once
#include "../properties/intrusive_ptr_counter.hpp"

namespace mystd {

    template<typename T>
    class IntrusivePtr {
        public:
            explicit IntrusivePtr(T* ptr)
                :ptr_(ptr)
            {
                mystd::IntrusivePtrCounter<T>::intrusive_ptr_add_ref(ptr);
            }

            IntrusivePtr(const IntrusivePtr& other)
                :ptr_(other.Get())
            {
                mystd::IntrusivePtrCounter<T>::intrusive_ptr_add_ref(ptr_);
            }

            IntrusivePtr& operator=(const IntrusivePtr& other) {
                if (this != &other) {
                    ptr_ = other.Get();
                    mystd::IntrusivePtrCounter<T>::intrusive_ptr_add_ref(ptr_);
                }
                return *this;
            }

            ~IntrusivePtr() {
                mystd::IntrusivePtrCounter<T>::intrusive_ptr_release(ptr_);
            }

            T* operator->() const {
                return ptr_;
            }

            T& operator*() const {
                return *ptr_;
            }

            void Reset(T* ptr) {
                mystd::IntrusivePtrCounter<T>::intrusive_ptr_release(ptr_);
                ptr_ = ptr;
                mystd::IntrusivePtrCounter<T>::intrusive_ptr_add_ref(ptr_);
            }

            T* Get() const {
                return ptr_;
            }

            size_t GetCount() const {
                return mystd::IntrusivePtrCounter<T>::GetCount(ptr_);
            }

        private:
            T* ptr_;
    };

} // namespace mystd