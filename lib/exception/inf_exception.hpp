#pragma once
#include <exception>
#include <concepts>

namespace mystd {
    template<typename T>
    requires (!std::is_reference_v<T>)
    class InfException : public std::exception {
        public:
            InfException(std::exception_ptr&& exception, const T& info) 
                requires std::is_copy_constructible_v<T>
                :exception_(std::move(exception))
                ,info_(info)
            {}

            InfException(std::exception_ptr&& exception, T&& info) 
                requires std::is_move_constructible_v<T>
                :exception_(std::move(exception))
                ,info_(std::move(info))
            {}

            virtual ~InfException() override = default;

            InfException(const InfException& other) = delete;

            InfException& operator=(const InfException& other) = delete;

            void set_info(auto&& info) noexcept 
            requires std::same_as<std::remove_reference_t<decltype(info)>, T>
            {
                info_ = std::forward<decltype(info)>(info);
            }

            void set_exception(std::exception_ptr&& exception) noexcept {
                exception_ = std::move(exception);
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