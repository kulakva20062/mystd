#pragma once
#include <cstddef>
#include <iostream>
#include "exception.h"

namespace mystd {
    
    template<typename Type>
    class Vector {
        public: 
            explicit Vector(size_t size = 0, Type default_variable = Type()) 
                :size(size)
                ,real_size(size + 1)
                ,array(new Type[size + 1])
            {
                for (size_t index = 0; index < size; ++index) {
                    array[index] = default_variable;
                }
            }

            ~Vector() {
                delete[] array;
            }

            Vector(const Vector<Type>& other)
                :size(other.size)
                ,real_size(other.real_size)
                ,array(new Type[real_size])
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
                    throw ex1;
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

            void Resize(size_t new_size) {
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

            void Resize(size_t new_size, Type value) {
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

            void Reverse(size_t begin, size_t end) {
                if (begin >= end || end > size) {
                    throw ex1;
                }
                for (size_t index1 = begin, index2 = end - 1;index1 < index2; ++index1, --index2) {
                    Swap(index1, index2);
                }
            }

            bool Empty() {
                return !(bool(size));
            }

            void PushBack(Type value) {
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
                    throw ex3;
                } 
                --size;
            }

            size_t Size() const {
                return size;
            }

            void Insert(size_t position, Type value) {
                if (position > size) {
                    throw ex1;
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
                for (size_t index = size; index > position; --index) {
                    array[index] = array[index - 1];
                }
                array[position] = value;    
                ++size;
            }

            void Erase(size_t position) {
                if (position >= size) {
                    throw ex1;
                }
                for (size_t index = position + 1; index < size; ++index) {
                    array[index - 1] = array[index]; 
                }
                --size;
            }

            Type Top() const {
                if (size == 0) {
                    throw ex3;
                }
                return array[size - 1];
            }

            void Swap(size_t position1, size_t position2) {
                if (position1 >= size || position2 >= size) {
                    throw ex1;
                } else {
                    Type other = array[position1];
                    array[position1] = array[position2];
                    array[position2] = other;
                }
            }

            void Print(const size_t size, const char* padding = " ", const bool line_feed = false) const {
                if (size >= this->size) {
                    throw ex1;
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

            void Sort(size_t begin, size_t end, bool (*function)(Type A, Type B)) {
                if (begin >= end || end > size) {
                    throw ex1;
                }
                size_t run = 64;
                for (size_t index = begin; index < end; index += run) {
                    if (index + run <= end) 
                        InsertionSort(index, index + run, function);
                    else 
                        InsertionSort(index, end, function);
                }
                for (;end - begin >= run; run <<= 1) {
                    Type* buffer = new Type[run]; 
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

            void Sort(size_t begin, size_t end) {
                if (begin >= end || end > size) {
                    throw ex1;
                }
                size_t run = 64;
                for (size_t index = begin; index < end; index += run) {
                    if (index + run <= end) 
                        InsertionSort(index, index + run);
                    else 
                        InsertionSort(index, end);
                }
                for (;(end - begin) > run; run <<= 1) {
                    Type* buffer = new Type[run * 2]; 
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

        protected:
            Exception ex1{"Передано значение за пределом вектора",  1}; 
            Exception ex2{"Попытка уменьшить вектор на размер меньше 0",  2};
            Exception ex3{"Вектор пуст Top() не работает", 3};
        private:
            Type* array;
            size_t size;
            size_t real_size;

            bool CompareLess(Type A, Type B) {
                return A <= B;
            } 

            void InsertionSort(size_t begin, size_t end, bool (*function)(Type A, Type B)) {
                for (size_t index = 1; index < end; ++index) {
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

            void InsertionSort(size_t begin, size_t end) {
                for (size_t index = 1; index < end; ++index) {
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