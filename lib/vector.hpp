#pragma once
#include <cstddef>
#include <iostream>
#include "exception.hpp"

namespace mystd {

    template<typename Type>
    class Vector {
        public:
            explicit Vector(size_t size = 0,const Type& default_variable = Type()) 
                :size(size)
                ,real_size(size + 1)
                ,array(new Type[size + 1])
            {
                for (size_t index = 0; index < size; ++index) {
                    array[index] = default_variable;
                }
            }

            virtual ~Vector() {
                delete[] array;
            }

            Vector(const Vector<Type>& other)
                :size(other.size)
                ,real_size(other.real_size)
                ,array(new Type[other.real_size])
            {
                for (size_t index = 0; index < size; index++) {
                    array[index] = other.array[index];
                }
            }

            Vector<Type>& operator=(const Vector<Type>& other) {
                if (&other == this) {
                    return *this;
                }
                delete[] array;
                size = other.size;
                real_size = other.real_size;
                array = new Type[real_size];
                for (size_t index = 0; index < size; index++) {
                    array[index] = other.array[index];
                }
                return *this;
            }

            Type& operator[](const size_t position) const {
                if (position >= size) {
                    throw Exception("Передано значение за пределом вектора operator[]", 1);
                }
                return array[position];
            }

            virtual Vector<Type>& operator+=(const Vector<Type>& other) {
                if (size + other.size > real_size) {
                    real_size = size + other.size;
                    Type* new_array = new Type[real_size];
                    for (size_t index = 0; index < size; ++index) {
                        new_array[index] = array[index];
                    }
                    delete[] array;
                    array = new_array;
                }
                for (size_t index = 0; index < other.size; ++index) {
                    array[size + index] = other.array[index];
                }
                size += other.size;
                return *this;
            }

            virtual Vector<Type> operator + (const Vector<Type>& other) const {
                Vector<Type> new_vector(size + other.size);
                for (size_t index = 0; index < size; ++index) {
                    new_vector[index] = array[index];
                }
                for (size_t index = 0; index < other.size; ++index) {
                    new_vector[index + size] = other.array[index];
                }
                return new_vector;
            }

            bool operator==(const Vector<Type>& other) const {
                if (this == &other) {
                    return true;
                }
                if (size != other.size) {
                    return false;
                } else {
                    bool is_equal = true;
                    for (size_t index = 0; index < size && is_equal; ++index) {
                        if (array[index] != other.array[index]) {
                            is_equal = false;
                        }
                    }
                    if (is_equal) return true;
                }
                return false;
            }

            bool operator!=(const Vector<Type>& other) const {
                if (this == &other) {
                    return false;
                }
                if (size != other.size) {
                    return true;
                } else {
                    bool is_equal = true;
                    for (size_t index = 0; index < size && is_equal; ++index) {
                        if (array[index] != other.array[index]) {
                            is_equal = false;
                        }
                    }
                    if (is_equal) return false;
                }
                return true;
            }

            bool operator>(const Vector<Type>& other) {
                for (size_t index = 0; index < other.size && index < size; ++index) {
                    if (array[index] > other[index]) return true;
                    else if (array[index] < other[index]) return false;
                }
                if (size < other.size) return true;
                return false;
            }

            bool operator<(const Vector<Type>& other) {
                for (size_t index = 0; index < other.size && index < size; ++index) {
                    if (array[index] > other[index]) return false;
                    else if (array[index] < other[index]) return true;
                }
                if (size > other.size) return true;
                return false;
            }

            bool operator>=(const Vector<Type>& other) {
                for (size_t index = 0; index < other.size && index < size; ++index) {
                    if (array[index] > other[index]) return true;
                    else if (array[index] < other[index]) return false;
                }
                if (size <= other.size) return true;
                return false;
            }

            bool operator<=(const Vector<Type>& other) {
                for (size_t index = 0; index < other.size && index < size; ++index) {
                    if (array[index] < other[index]) return true;
                    else if (array[index] > other[index]) return false;
                }
                if (size < other.size) return false;
                return true;
            }

            void Resize(const size_t new_size) {
                if (size >= new_size) {
                    size = new_size;
                } else {
                    if (real_size < new_size) {
                        real_size = new_size;
                        Type* new_array = new Type[real_size];
                        for (size_t index = 0; index < size; ++index) {
                            new_array[index] = array[index];
                        }
                        delete[] array;
                        array = new_array;
                    }
                    size = new_size;
                }
            }

            void Resize(const size_t new_size, const Type& value) {
                if (size >= new_size) {
                    size = new_size;
                } else {
                    if (real_size < new_size) {
                        real_size = new_size;
                        Type* new_array = new Type[real_size];
                        for (size_t index = 0; index < size; ++index) {
                            new_array[index] = array[index];
                        }
                        delete[] array;
                        array = new_array;
                    }
                    for (size_t index = size; index < new_size; ++index) {
                        array[index] = value;
                    }
                    size = new_size;
                }
            }

            void Reverse() {
                for (size_t index1 = 0, index2 = size - 1; index1 < index2; ++index1, --index2) {
                    Swap(index1, index2);
                }
            }

            void Reverse(const size_t begin, const size_t end) {
                if (begin > end || end > size) {
                    throw Exception("Передано значение за пределом вектора Reverse", 1);
                }
                if (begin == end) return;
                for (size_t index1 = begin, index2 = end - 1;index1 < index2; ++index1, --index2) {
                    Swap(index1, index2);
                }
            }

            bool Empty() const {
                return !size;
            }

            void PushBack(const Type& value = Type()) {
                if (size == real_size) {
                    real_size *= 2;
                    Type* new_array = new Type[real_size];
                    for (size_t index = 0; index < size; ++index) {
                        new_array[index] = array[index];
                    }
                    delete[] array;
                    array = new_array;
                }
                array[size] = value;
                ++size;
            }

            void PopBack() {
                if (size == 0) {
                    throw Exception("Передано значение за пределом вектора PopBack", 1);
                }
                --size;
            }

            size_t Size() const {
                return size;
            }

            void Insert(const size_t position, const Type& value = Type()) {
                if (position > size) {
                    throw Exception("Передано значение за пределом вектора Insert", 1);
                }
                if (size == position) {
                    PushBack(value);
                    return;
                }
                if (size == real_size) {
                    real_size *= 2;
                    Type* new_array = new Type[real_size];
                    for (size_t index = 0; index < size; ++index) {
                        new_array[index] = array[index];
                    }
                    delete[] array;
                    array = new_array;
                }
                ++size;
                for (size_t index = size - 1; index > position; --index) {
                    array[index] = array[index - 1];
                }
                array[position] = value;
            }

            void Insert(const size_t begin, const size_t end, const Type& value = Type()) {
                if (begin > size || begin >= end) {
                    throw Exception("Передано значение за пределом вектора Insert", 1);
                }
                if (size == begin) {
                    for (size_t index = 0; index < end - begin; ++index) {
                        PushBack(value);
                    }
                    return;
                }
                size += end - begin;
                while (size > real_size) {
                    real_size *= 2;
                    Type* new_array = new Type[real_size];
                    for (size_t index = 0; index < size - (end - begin); ++index) {
                        new_array[index] = array[index];
                    }
                    delete[] array;
                    array = new_array;
                }
                for (size_t index = size - 1; index >= end; --index) {
                    array[index] = array[index - (end - begin)];
                }
                for (size_t index = begin; index < end; ++index) {
                    array[index] = value;
                }
            }

            void Erase(const size_t position) {
                if (position >= size) {
                    throw Exception("Передано значение за пределом вектора Erase", 1);
                }
                for (size_t index = position + 1; index < size; ++index) {
                    array[index - 1] = array[index];
                }
                --size;
            }

            void Erase(const size_t begin, const size_t end) {
                if (end > size || begin >= end) {
                    throw Exception("Передано значение за пределом вектора Erase", 1);
                }
                for (size_t index = end; index < size; ++index) {
                    array[index - (end - begin)] = array[index];
                }
                size -= (end - begin);
            } // протестировано.

            Type Top() const {
                if (size == 0) {
                    throw Exception("Передано значение за пределом вектора Top", 1);
                }
                return array[size - 1];
            }

            size_t Begin() const {
                return 0;
            }

            size_t End() const {
                return size;
            }

            void Clear() {
                delete[] array;
                size = 0;
                array = new Type[size + 1];
                real_size = size + 1;
            }

            void Swap(const size_t position1, const size_t position2) {
                if (position1 >= size || position2 >= size) {
                    throw Exception("Передано значение за пределом вектора Swap", 1);
                } else {
                    Type other = array[position1];
                    array[position1] = array[position2];
                    array[position2] = other;
                }
            }

            void Print(const size_t size, const char* padding = " ", const bool line_feed = false) const {
                if (size > this->size) {
                    throw Exception("Передано значение за пределом вектора Print", 1);
                }
                for (size_t index = 0; index < size; ++index) {
                    std::cout << array[index] << padding;
                }
                if (line_feed) {
                    std::cout << '\n';
                }
            }

            void Print(const char* padding = " ", const bool line_feed = true) const {
                for (size_t index = 0; index < size; ++index) {
                    std::cout << array[index] << padding;
                }
                if (line_feed) {
                    std::cout << '\n';
                }
            }

            void Sort(const size_t begin, const size_t end, bool (*function)(const Type& A, const Type& B)) {
                if (begin >= end || end > size) {
                    throw Exception("Передано значение за пределом вектора Sort", 1);
                }
                size_t run = 64;
                for (size_t index = begin; index < end; index += run) {
                    if (index + run <= end)
                        InsertionSort(index, index + run, function);
                    else
                        InsertionSort(index, end, function);
                }
                for (;end - begin >= run; run <<= 1) {
                    Type* buffer = new Type[run << 1];
                    for (size_t index = begin; index + run < end; index += (run << 1)) {
                        size_t index1 = index;
                        size_t index2 = index + run;
                        size_t pos = 0;

                        for(; (index1 < index + run) && (index2 < index + (run << 1)) && (index2 != end); ++pos) {
                            if (function(array[index1], array[index2])) {
                                buffer[pos] = array[index1];
                                ++index1;
                            } else {
                                buffer[pos] = array[index2];
                                ++index2;
                            }
                        }
                        for(; index1 < index + run; ++index1, ++pos) {
                            buffer[pos] = array[index1];
                        }
                        for(; (index2 < index + (run << 1)) && (index2 != end); ++index2, ++pos) {
                            buffer[pos] = array[index2];
                        }

                        index1 = index;
                        for (size_t index = 0; index < pos; ++index, ++index1) {
                            array[index1] = buffer[index];
                        }

                    }
                    delete[] buffer;
                }
            }

            void Sort(const size_t begin, const size_t end) {
                if (begin >= end || end > size) {
                    throw Exception("Передано значение за пределом вектора Sort", 1);
                }
                size_t run = 64;
                for (size_t index = begin; index < end; index += run) {
                    if (index + run <= end)
                        InsertionSort(index, index + run);
                    else
                        InsertionSort(index, end);
                }
                for (;(end - begin) > run; run <<= 1) {
                    Type* buffer = new Type[run << 1];
                    for (size_t index = begin; index + run < end; index += (run << 1)) {
                        size_t index1 = index;
                        size_t index2 = index + run;
                        size_t pos = 0;

                        for(; (index1 < index + run) && (index2 < index + (run << 1)) && (index2 < end); ++pos) {
                            if (CompareLess(array[index1], array[index2])) {
                                buffer[pos] = array[index1];
                                ++index1;
                            } else {
                                buffer[pos] = array[index2];
                                ++index2;
                            }
                        }
                        for(; index1 < index + run; ++index1, ++pos) {
                            buffer[pos] = array[index1];
                        }
                        for(; (index2 < index + (run << 1)) && (index2 < end); ++index2, ++pos) {
                            buffer[pos] = array[index2];
                        }

                        index1 = index;
                        for (size_t index = 0; index < pos; ++index, ++index1) {
                            array[index1] = buffer[index];
                        }

                    }
                    delete[] buffer;
                }
            }

            void Sort() {
                size_t run = 64;
                for (size_t index = 0; index < size; index += run) {
                    if (index + run <= size)
                        InsertionSort(index, index + run);
                    else
                        InsertionSort(index, size);
                }
                for (;size > run; run <<= 1) {
                    Type* buffer = new Type[run << 1];
                    for (size_t index = 0; index + run < size; index += (run << 1)) {
                        size_t index1 = index;
                        size_t index2 = index + run;
                        size_t pos = 0;

                        for(; (index1 < index + run) && (index2 < index + (run << 1)) && (index2 < size); ++pos) {
                            if (CompareLess(array[index1], array[index2])) {
                                buffer[pos] = array[index1];
                                ++index1;
                            } else {
                                buffer[pos] = array[index2];
                                ++index2;
                            }
                        }
                        for(; index1 < index + run; ++index1, ++pos) {
                            buffer[pos] = array[index1];
                        }
                        for(; (index2 < index + (run << 1)) && (index2 < size); ++index2, ++pos) {
                            buffer[pos] = array[index2];
                        }

                        index1 = index;
                        for (size_t index = 0; index < pos; ++index, ++index1) {
                            array[index1] = buffer[index];
                        }

                    }
                    delete[] buffer;
                }
            }

            void Sort(bool (*function)(const Type& A, const Type& B)){
                size_t run = 64;
                for (size_t index = 0; index < size; index += run) {
                    if (index + run <= size)
                        InsertionSort(index, index + run, function);
                    else
                        InsertionSort(index, size, function);
                }
                for (;size > run; run <<= 1) {
                    Type* buffer = new Type[run << 1];
                    for (size_t index = 0; index + run < size; index += (run << 1)) {
                        size_t index1 = index;
                        size_t index2 = index + run;
                        size_t pos = 0;

                        for(; (index1 < index + run) && (index2 < index + (run << 1)) && (index2 < size); ++pos) {
                            if (function(array[index1], array[index2])) {
                                buffer[pos] = array[index1];
                                ++index1;
                            } else {
                                buffer[pos] = array[index2];
                                ++index2;
                            }
                        }
                        for(; index1 < index + run; ++index1, ++pos) {
                            buffer[pos] = array[index1];
                        }
                        for(; (index2 < index + (run << 1)) && (index2 < size); ++index2, ++pos) {
                            buffer[pos] = array[index2];
                        }

                        index1 = index;
                        for (size_t index = 0; index < pos; ++index, ++index1) {
                            array[index1] = buffer[index];
                        }

                    }
                    delete[] buffer;
                }
            }

            Vector<Type> SubLine(const size_t begin, const size_t end) const {
                if (begin > end || end > size) {
                    throw Exception("Передано значение за пределом вектора, SubLine", 1);
                }
                Vector<Type> new_vector(end - begin);
                for (size_t index = 0; index < new_vector.Size(); ++index) {
                    new_vector[index] = array[index + begin];
                }
                return new_vector;
            }

            size_t Count(const Type& value) const {

                size_t quantity = 0;
                for (size_t index = 0; index < size; ++index) {
                    if (array[index] == value) {
                        ++quantity;
                    }
                }
                return quantity;
            }

            size_t Count(const size_t begin, const size_t end, const Type& value) const {
                if (begin >= end || end > size) {
                    throw Exception("Передано значение за пределом вектора, Count", 1);
                }
                size_t quantity = 0;
                for (size_t index = begin; index < end; ++index) {
                    if (array[index] == value) {
                        ++quantity;
                    }
                }
                return quantity;
            }

            size_t Find(const Type& value) const {
                size_t index = 0;
                while (index < size) {
                    if (array[index] == value) return index;
                    ++index;
                }
                return index;
            }

            size_t Find(size_t begin, size_t end, const Type& value) const {
                if (begin >= end || end > size) {
                    throw Exception("Передано значение за пределом вектора, Find", 1);
                }
                size_t index = begin;
                while (index < end) {
                    if (array[index] == value) return index;
                    ++index;
                }
                return index;
            }

            size_t RFind(const Type& value) const {
                if (size == 0) {
                    return 0;
                }
                size_t index = size - 1;
                while (index > 0) {
                    if (array[index] == value) return index;
                    --index;
                }
                if (array[index] != value) index = size;
                return index;
            }

            size_t RFind(size_t begin, size_t end, const Type& value) const {
                if (begin >= end || end > size) {
                    throw Exception("Передано значение за пределом вектора RFind", 1);
                }
                size_t index = end - 1;
                while (index > begin) {
                    if (array[index] == value) return index;
                    --index;
                }
                if (array[index] != value) index = end;
                return index;
            }
            
        private:
            Type* array;
            size_t size;
            size_t real_size;

            bool CompareLess(const Type& A,const Type& B) const {
                return A < B;
            }

            void InsertionSort(const size_t begin, const size_t end, bool (*function)(const Type& A, const Type& B)) {
                for (size_t index = begin + 1; index < end; ++index) {
                    Type current = array[index];
                    size_t left = begin;
                    size_t right = index;
                    while (left < right) {
                        size_t mid = left + (right - left) / 2;

                        if (function(current, array[mid])) {
                            right = mid;
                        } else {
                            left = mid + 1;
                        }
                    }
                    for (size_t index_ = index; index_ > left; --index_) {
                        array[index_] = array[index_ - 1];
                    }
                    array[left] = current;
                }
            }

            void InsertionSort(const size_t begin, const size_t end) {
                for (size_t index = begin + 1; index < end; ++index) {
                    Type current = array[index];
                    size_t left = begin;
                    size_t right = index;
                    while (left < right) {
                        size_t mid = left + (right - left) / 2;

                        if (CompareLess(current, array[mid])) {
                            right = mid;
                        } else {
                            left = mid + 1;
                        }
                    }
                    for (size_t index_ = index; index_ > left; --index_) {
                        array[index_] = array[index_ - 1];
                    }
                    array[left] = current;
                }
            }
    };
    
} // namespace mystd
