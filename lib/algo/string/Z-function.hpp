#pragma once
#include <iterator>
#include <vector>
#include <algorithm>

template<typename it>
requires std::random_access_iterator<it>
std::vector<size_t> Zfunc(it begin, it end) {
    std::vector<size_t> result(end - begin, 0);
    if (begin == end) return result;
    it start = begin;

    it left= begin;
    it right= begin;
    ++begin;
    size_t index = 1;
    while (begin != end) { 
        if (begin <= right) {
            result[index] = std::min(size_t(right - begin + 1), result[static_cast<size_t>(begin - left)]);
        }
        while (index + result[index] < end - start && *(begin + result[index]) == *(start + result[index])) {
            ++result[index];
        }
        if (begin + result[index] > right) {
            left = begin;
            right = begin + result[index] - 1;
        }
        ++begin;
        ++index;
    }
    
    return result;
}

