#pragma once
#include <cstddef>
#include "exception.hpp"

namespace mystd {

    template<typename Type> 
    class Queue {
        public:
            Queue(const size_t size = 0, const Type& default_value = Type())
                :size(size)
                ,first_in_line(nullptr)
                ,last_in_line(nullptr)
            {
                if (size != 0) {
                    last_in_line = new Node{.data = default_value};
                    first_in_line = last_in_line;
                }
                for (size_t index = 1; index < size; ++index) {
                    last_in_line->next_node = new Node{.data = default_value};
                    last_in_line = last_in_line->next_node;
                }
            }

            ~Queue() {
                DeleteQueue();
            }

            Queue(const Queue<Type>& other) 
                :size(other.size)
                ,first_in_line(nullptr)
                ,last_in_line(nullptr)
            {
                Node* cur_node = other.first_in_line;
                if (size != 0) {
                    last_in_line = new Node{.data = cur_node->data};
                    first_in_line = last_in_line;
                }
                for (size_t index = 1; index < size; ++index) {
                    cur_node = cur_node->next_node;
                    last_in_line->next_node = new Node{.data = cur_node->data};
                    last_in_line = last_in_line->next_node;
                }
            }

            Queue(Queue<Type>&& other) noexcept
                :size(other.size)
                ,first_in_line(other.first_in_line)
                ,last_in_line(other.last_in_line)
            {
                other.size = 0;
                other.first_in_line = nullptr;
                other.last_in_line = nullptr;
            }

            Queue& operator=(const Queue<Type>& other) {
                if (this == &other) {
                    return *this;
                }
                DeleteQueue();
                size = other.size;
                Node* cur_node = other.first_in_line;
                if (size != 0) {
                    last_in_line = new Node{.data = cur_node->data};
                    first_in_line = last_in_line;
                }
                for (size_t index = 1; index < size; ++index) {
                    cur_node = cur_node->next_node;
                    last_in_line->next_node = new Node{.data = cur_node->data};
                    last_in_line = last_in_line->next_node;
                }
                return *this;
            }

            Queue& operator=(Queue<Type>&& other) noexcept {
                if (this == &other) {
                    return *this;
                }
                DeleteQueue();
                size = other.size;
                first_in_line = other.first_in_line;
                last_in_line = other.last_in_line;
                other.size = 0;
                other.first_in_line = nullptr;
                other.last_in_line = nullptr;
                return *this;
            }

            void Push(const Type& value) {
                if (size == 0) {
                    last_in_line = new Node{.data = value};
                    first_in_line = last_in_line;
                } else {
                    last_in_line->next_node = new Node{.data = value};
                    last_in_line = last_in_line->next_node;
                }
                ++size;
                
            }

            void Pop() {
                if (size == 0) {
                    throw Exception("Попытка обратиться к очереди с 0 элементов(Pop, Front)", 3);
                }
                Node* next_node = first_in_line->next_node;
                delete first_in_line;
                first_in_line = next_node;
                --size;
                if (size == 0) {
                    last_in_line = nullptr;
                }
            }

            const Type& Front() const {
                if (size == 0) {
                    throw Exception("Попытка обратиться к очереди с 0 элементов(Pop, Front)", 3);
                }
                return first_in_line->data;
            }

            size_t Size() const {
                return size;
            }

            struct Node {
                Node* next_node = nullptr;

                Type data;
            };

        private:
            Node* first_in_line;
            Node* last_in_line;
            size_t size;

            void DeleteQueue() {
                while (first_in_line != nullptr) {
                    Node* next_node = first_in_line->next_node;
                    delete first_in_line;
                    first_in_line = next_node;
                } 
                last_in_line = nullptr;
            }
    };

}