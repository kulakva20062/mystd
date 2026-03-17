#pragma once
#include <compare>
#include <concepts>
#include <unordered_map>
#include <optional>
#include <vector>

template<typename Maps>
concept MapContainer = requires {
    typename Maps::key_type;
    typename Maps::mapped_type;
    typename Maps::value_type;
    requires std::same_as<typename Maps::value_type, std::pair<const typename Maps::key_type, typename Maps::mapped_type>>;
} && requires(Maps m, const Maps cm, typename Maps::key_type k) {
    { m.find(k) } -> std::same_as<typename Maps::iterator>;
    { cm.find(k) } -> std::same_as<typename Maps::const_iterator>;
    { m.begin() } -> std::same_as<typename Maps::iterator>;
    { m.end() } -> std::same_as<typename Maps::iterator>;
    { cm.begin() } -> std::same_as<typename Maps::const_iterator>;
    { cm.end() } -> std::same_as<typename Maps::const_iterator>;
    { m[k] } -> std::same_as<typename Maps::mapped_type&>;
};

template<typename T, template<typename, typename> class maps = std::unordered_map>
requires MapContainer<maps<T, int>>
class Bor {
        template<template<typename> class container, bool IsConst>
        class BorIterator;
        struct Node;

    public:
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = BorIterator<std::vector, false>;
        using const_iterator = BorIterator<std::vector, true>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
    
        Bor() {
            root_ = new Node();
        }

        ~Bor() {
            delete root_;
        }

        template<template<typename> class container>
        void insert(const container<value_type>& keys) {
            Node* current = root_;
            for (const auto& key : keys) {
                if (current->children.find(key) == current->children.end()) {
                    current->children[key] = new Node();
                }
                current = current->children[key];
            }
            if (!current->is_end) {
                ++size_;
                current->is_end = true;
            }
        }

        template<template<typename> class container>
        void erase(const container<value_type>& keys) {
            if (auto current = find_pointer(keys)) {
                if ((*current)->is_end) {
                    --size_;
                    (*current)->is_end = false;
                }
            }
        }

        template<template<typename> class container>
        bool find(const container<value_type>& keys) const {
            Node* current = root_;
            for (const auto& key : keys) {
                if (current->children.find(key) == current->children.end()) {
                    return false;
                }
                current = current->children[key];
            }
            return current->is_end;
        }

        template<template<typename> class container_input, template<typename> class container_output1 = std::vector, template<typename> class container_output2 = std::vector>
        container_output1<container_output2<value_type>> find_full(const container_input<value_type>& keys) const {
            container_output1<container_output2<value_type>> result;
            Node* current = root_;
            for (const auto& key : keys) {
                if (current->children.find(key) == current->children.end()) {
                    return result;
                }
                current = current->children[key];
            }
            container_output2<value_type> prefix(keys.begin(), keys.end());
            collect_all<container_output1, container_output2>(current, prefix, result);
            return result;
        }

        size_type size() const { return size_; }

        bool empty() const { return size_ == 0; }

        template<template<typename> class container = std::vector>
        BorIterator<container, false> begin() {
            return BorIterator<container, false>(root_, 0);
        }

        template<template<typename> class container = std::vector>
        BorIterator<container, false> end() {
            return BorIterator<container, false>(nullptr, size_);
        }

        template<template<typename> class container = std::vector>
        BorIterator<container, true> cbegin() const {
            return BorIterator<container, true>(root_, 0);
        }

        template<template<typename> class container = std::vector>
        BorIterator<container, true> cend() const {
            return BorIterator<container, true>(nullptr, size_);
        }

        template<template<typename> class container = std::vector>
        BorIterator<container, true> begin() const {
            return cbegin<container>();
        }

        template<template<typename> class container = std::vector>
        BorIterator<container, true> end() const {
            return cend<container>();
        }

    private:
        size_type size_ = 0;
        Node* root_;

        struct Node {
            maps<value_type, Node*> children;
            bool is_end;

            ~Node() {
                for (auto& child : children) {
                    delete child.second;
                }
            }
        };  

        template<template<typename> class container_output1, template<typename> class container_output2>
        void collect_all(Node* node, container_output2<value_type>& current, container_output1<container_output2<value_type>>& result) const {
            if (node->is_end) {
                result.push_back(current);
            }
            for (auto& [key, child] : node->children) {
                current.push_back(key);
                collect_all<container_output1, container_output2>(child, current, result);
                current.pop_back();
            }
        }

        template<template<typename> class container>
        std::optional<Node*> find_pointer(const container<value_type>& keys) {
            Node* current = root_;
            for (const auto& key : keys) {
                if (current->children.find(key) == current->children.end()) {
                    return std::nullopt;
                }
                current = current->children[key];
            }
            return std::make_optional(current);
        }

        template<template<typename> class container, bool IsConst>
        class BorIterator {
            public:
                using value_type = T;
                using reference = std::conditional_t<IsConst, const value_type&, value_type&>;
                using pointer = std::conditional_t<IsConst, const Node*, Node*>;
                using difference_type = ptrdiff_t;
                using size_type = size_t;
                using iterator_category = std::forward_iterator_tag;

                BorIterator(Node* root, size_type index)
                    : index_(index)
                    , root_(root)
                {
                    if (root_ && !root_->children.empty()) {
                        begin_.push_back(root_->children.begin());
                        end_.push_back(root_->children.end());
                        if (!begin_.back()->second->is_end) {
                            Descend();
                        }
                    }
                }

                bool operator==(const BorIterator& other) const {
                    return index_ == other.index_;
                }

                std::strong_ordering operator<=>(const BorIterator& other) const {
                    return index_ <=> other.index_;
                }

                BorIterator& operator++() {
                    ++index_;
                    Next();
                    return *this;
                }

                BorIterator operator++(int) {
                    BorIterator tmp = *this;
                    ++(*this);
                    return tmp;
                }

                container<value_type> operator*() const {
                    container<value_type> result;
                    for (const auto& it : begin_) {
                        result.push_back(it->first);
                    }
                    return result;
                }

            private:
                size_type index_;
                std::vector<typename maps<value_type, Node*>::iterator> begin_;
                std::vector<typename maps<value_type, Node*>::iterator> end_;
                Node* root_;

                bool Descend() {
                    Node* node = begin_.back()->second;
                    if (node->children.empty()) {
                        return false;
                    }
                    begin_.push_back(node->children.begin());
                    end_.push_back(node->children.end());
                    if (begin_.back()->second->is_end) {
                        return true;
                    }
                    return Descend();
                }

                bool Next() {
                    if (!begin_.empty() && Descend()) {
                        return true;
                    }
                    while (!begin_.empty()) {
                        ++begin_.back();
                        while (begin_.back() != end_.back()) {
                            if (begin_.back()->second->is_end) {
                                return true;
                            }
                            if (Descend()) {
                                return true;
                            }
                            ++begin_.back();
                        }
                        begin_.pop_back();
                        end_.pop_back();
                    }
                    return false;
                }
        };

};