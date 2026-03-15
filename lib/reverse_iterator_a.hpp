#pragma once
#include <iterator>

template<typename it>
class reverse_iterator_a {
public:
    using iterator_type = it;
    using iterator_category = typename std::iterator_traits<it>::iterator_category;
    using value_type = typename std::iterator_traits<it>::value_type;
    using difference_type = typename std::iterator_traits<it>::difference_type;
    using pointer = typename std::iterator_traits<it>::pointer;
    using reference = typename std::iterator_traits<it>::reference;

    explicit reverse_iterator_a(it iterator)
        :current_(iterator) 
    {}

    template<typename Other>
    requires std::convertible_to<Other, it>
    reverse_iterator_a(const reverse_iterator_a<Other>& other)
        :current_(other.base()) 
    {}

    inline it base() const {
        return current_; 
    }

    reference operator*() const {
        it tmp = current_;
        return *(--tmp);
    }

    pointer operator->() const {
        it tmp = current_;
        --tmp;
        return std::addressof(*tmp); 
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    reverse_iterator_a& operator++() {
        --current_;
        return *this;
    }

    reverse_iterator_a operator++(int) {
        reverse_iterator_a tmp = *this;
        --current_;
        return tmp;
    }

    reverse_iterator_a& operator--() {
        ++current_;
        return *this;
    }

    reverse_iterator_a operator--(int) {
        reverse_iterator_a tmp = *this;
        ++current_;
        return tmp;
    }

    reverse_iterator_a& operator+=(difference_type n) {
        current_ -= n;
        return *this;
    }

    reverse_iterator_a& operator-=(difference_type n) {
        current_ += n;
        return *this;
    }

    reverse_iterator_a operator+(difference_type n) const {
        return reverse_iterator_a(current_ - n);
    }

    reverse_iterator_a operator-(difference_type n) const {
        return reverse_iterator_a(current_ + n);
    }

    friend reverse_iterator_a operator+(difference_type n, const reverse_iterator_a& iterator) {
        return iterator + n;
    }

    difference_type operator-(const reverse_iterator_a& other) const {
        return other.base() - current_;
    }

    template<typename Other>
    requires std::convertible_to<Other, it>
    friend difference_type operator-(const reverse_iterator_a& a, const reverse_iterator_a<Other>& b) {
        return b.base() - a.current_;
    }

    template<typename Other>
    requires std::convertible_to<Other, it>
    bool operator==(const reverse_iterator_a<Other>& other) const {
        return current_ == other.base();
    }

    template<typename Other>
    requires std::convertible_to<Other, it>
    std::strong_ordering operator<=>(const reverse_iterator_a<Other>& other) const {
        return other.base() <=> current_;
    }

private:
    it current_;
};