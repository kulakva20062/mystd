#pragma once
#include <exception>
#include <concepts>

namespace mystd {

    template<typename T>
    requires (!std::is_reference_v<T>)
    class InfException {
        public:
            InfException(const std::exception_ptr& exception, const T& info) noexcept
                requires std::is_copy_constructible_v<T>
                :exception_(exception)
                ,info_(info)
            {}

            InfException(const std::exception_ptr& exception, T&& info) noexcept
                requires std::is_move_constructible_v<T>
                :exception_(exception)
                ,info_(std::move(info))
            {}

            InfException(std::exception_ptr&& exception, const T& info) noexcept
                requires std::is_copy_constructible_v<T>
                :exception_(std::move(exception))
                ,info_(info)
            {}

            InfException(std::exception_ptr&& exception, T&& info) noexcept
                requires std::is_move_constructible_v<T>
                :exception_(std::move(exception))
                ,info_(std::move(info))
            {}

            ~InfException() = default;

            InfException(const InfException& other) noexcept
                :exception_(other.get_exception())
                ,info_(other.get_info())
            {}

            InfException(InfException&& other) noexcept 
                :exception_(std::move(other.get_exception()))
                ,info_(std::move(other.get_info()))
            {}

            InfException& operator=(const InfException& other) noexcept {
                if (this == &other) {
                    return *this;
                }
                exception_ = other.get_exception();
                info_ = other.get_info();
                return *this;
            }

            InfException& operator=(InfException&& other) noexcept {
                if (this == &other) {
                    return *this;
                }
                exception_ = std::move(other.get_exception());
                info_ = std::move(other.get_info());
                return *this;
            }

            void set_info(auto&& info) noexcept 
            requires std::same_as<std::remove_reference_t<decltype(info)>, T>
            {
                info_ = std::forward<decltype(info)>(info);
            }

            void set_exception(std::exception_ptr&& exception) noexcept {
                exception_ = std::move(exception);
            }

            void set_exception(std::exception_ptr& exception) noexcept {
                exception_ = exception;
            }
            
            const T& get_info() noexcept {
                return info_;
            }

            const std::exception_ptr& get_exception() noexcept {
                return exception_;
            }

        private:
            std::exception_ptr exception_;
            T info_;
    };
    
}