#pragma once
#include <cstddef>
#include "../../exception.hpp"

namespace mystd {

    template<typename Type> 
    class Deque {
        public:
            Deque(const size_t size = 0, const Type& default_value = Type())
                :size(size)
                ,first_in_line(nullptr)
                ,last_in_line(nullptr)
            {
                for (size_t index = 0; index < size; ++index) {
                    if (first_in_line == nullptr) {
                        first_in_line = new Node(default_value);
                        last_in_line = first_in_line;
                    } else {
                        last_in_line->next_node = new Node(default_value, last_in_line);
                        last_in_line = last_in_line->next_node;
                    }
                }
            }

            ~Deque() {
                DeleteDeque();
            }

            Deque(const Deque<Type>& other)
                :size(other.size)
                ,first_in_line(nullptr)
                ,last_in_line(nullptr)
            {
                Node* cur_node = other.first_in_line;
                for (size_t index = 0; index < size; ++index) {
                    if (first_in_line == nullptr) {
                        first_in_line = new Node(cur_node->data);
                        last_in_line = first_in_line;
                    } else {
                        last_in_line->next_node = new Node(cur_node->data, last_in_line);
                        last_in_line = last_in_line->next_node;
                    }
                    cur_node = cur_node->next_node;
                }
            }

            Deque(Deque<Type>&& other) noexcept
                :size(other.size)
                ,first_in_line(other.first_in_line)
                ,last_in_line(other.last_in_line)
            {
                other.size = 0;
                other.first_in_line = nullptr;
                other.last_in_line = nullptr;
            }

            Deque& operator=(const Deque<Type>& other) {
                if (this == &other) {
                    return *this;
                }
                DeleteDeque();
                size = other.size;
                Node* cur_node = other.first_in_line;
                for (size_t index = 0; index < size; ++index) {
                    if (first_in_line == nullptr) {
                        first_in_line = new Node(cur_node->data);
                        last_in_line = first_in_line;
                    } else {
                        last_in_line->next_node = new Node(cur_node->data, last_in_line);
                        last_in_line = last_in_line->next_node;
                    }
                    cur_node = cur_node->next_node;
                }
                return *this;
            }

            Deque& operator=(Deque<Type>&& other) noexcept {
                if (this == &other) {
                    return *this;
                }
                DeleteDeque();
                size = other.size;
                first_in_line = other.first_in_line;
                last_in_line = other.last_in_line;
                other.size = 0;
                other.first_in_line = nullptr;
                other.last_in_line = nullptr;
                return *this;
            }

            size_t Size() const {
                return size;
            }

            void PushBack(const Type& value) {
                if (last_in_line == nullptr) {
                    last_in_line = new Node(value);
                    first_in_line = last_in_line;
                } else {
                    last_in_line->next_node = new Node(value, last_in_line);
                    last_in_line = last_in_line->next_node;
                }
                ++size;
            }

            void PopBack() {
                if (size == 0) {
                    throw Exception("Попытка обратиться к Деку с 0 элементов(Pop, Front, Back)", 4);
                } else {
                    --size;
                    Node* new_node = last_in_line->last_node;
                    delete last_in_line;
                    last_in_line = new_node;
                    if (last_in_line != nullptr) {
                        last_in_line->next_node = nullptr;
                    } else {
                        first_in_line = nullptr;
                    }
                }
            }

            const Type& Back() const {
                if (size == 0) {
                    throw Exception("Попытка обратиться к Деку с 0 элементов(Pop, Front, Back)", 4);
                } else {
                    return last_in_line->data;
                }
            } 

            void PushFront(const Type& value) {
                if (first_in_line == nullptr) {
                    first_in_line = new Node(value);
                    last_in_line = first_in_line;
                } else {
                    first_in_line->last_node = new Node(value, nullptr, first_in_line);
                    first_in_line = first_in_line->last_node;
                }
                ++size;
            }

            void PopFront() {
                if (size == 0) {
                    throw Exception("Попытка обратиться к Деку с 0 элементов(Pop, Front, Back)", 4);
                } else {
                    --size;
                    Node* new_node = first_in_line->next_node;
                    delete first_in_line;
                    first_in_line = new_node;
                    if (first_in_line != nullptr) {
                        first_in_line->last_node = nullptr;
                    } else {
                        last_in_line = nullptr;
                    }
                }
            }

            const Type& Front() const {
                if (size == 0) {
                    throw Exception("Попытка обратиться к Деку с 0 элементов(Pop, Front, Back)", 4);
                } else {
                    return first_in_line->data;
                }
            }

            struct Node {
                Node* next_node = nullptr;
                Node* last_node = nullptr;

                Type data;
                Node(const Type& value = Type(), Node* last_node = nullptr,
                Node* first_node = nullptr) 
                    :data(value)
                    ,last_node(last_node)
                {}
            };

        private:
            Node* first_in_line;
            Node* last_in_line;
            size_t size;

            void DeleteDeque() {
                while (first_in_line != nullptr) {
                    Node* next_node = first_in_line->next_node;
                    delete first_in_line;
                    first_in_line = next_node;
                } 
                last_in_line = nullptr;
            }
    };

}