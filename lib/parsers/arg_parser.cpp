#include "arg_parser.hpp"
#include "../exception.hpp"
#include <string>
#include <vector>
#include <cstddef>
#include <cstring>

namespace mystd {

    Parser::Parser ()
        :start_node_(nullptr)
        ,last_node_(nullptr)
        ,size_(0)
    {}

    Parser::~Parser () {
        while(start_node_ != nullptr) {
            Node* node_argument = start_node_;
            start_node_ = start_node_->next_node;
            delete node_argument;
        }
    }

    void Parser::Parse(int argc, const char** argv) {
        std::vector<bool> is_reading(argc, false);
        
        for (size_t index = 1; index < argc; ++index) {
            Node* cur_node = start_node_;
            std::string string(argv[index]);
            std::vector<Node*> is_involved;
            
            //Проверка на команду
            while (cur_node != nullptr) {
                if (string.size() > cur_node->small_name.size()) {
                    if (cur_node->small_name == string.substr(0, cur_node->small_name.size())
                        && string[cur_node->small_name.size()] == '=') {
                        is_reading[index] = true;
                        cur_node->is_involved = true;
                        if (cur_node->narg != kCommandOnly) {
                            if (cur_node->narg == kDelayedOptional || cur_node->narg == kImmediateOptional
                                && cur_node->vector.size() == 0) {
                                cur_node->vector.push_back(string.substr(cur_node->small_name.size() + 1));
                            }
                            if (cur_node->narg != kDelayedOptional && cur_node->narg != kImmediateOptional) {
                                cur_node->to_read = true;
                                is_involved.push_back(cur_node);
                            }
                        }
                    }
                } 
                else if (string.size() == cur_node->small_name.size()) {
                    if (cur_node->small_name == string) {
                        is_reading[index] = true;
                        cur_node->is_involved = true;
                        if (cur_node->narg != kCommandOnly) {
                            cur_node->to_read = true;
                            is_involved.push_back(cur_node);
                        }
                    }
                }
                if (string.size() > cur_node->long_name.size()) {
                    if (cur_node->long_name == string.substr(0, cur_node->long_name.size())
                        && string[cur_node->long_name.size()] == '=') {
                        is_reading[index] = true;
                        cur_node->is_involved = true;
                        if (cur_node->narg != kCommandOnly) {
                            if (cur_node->narg == kDelayedOptional || cur_node->narg == kImmediateOptional
                                && cur_node->vector.size() == 0) {
                                cur_node->vector.push_back(string.substr(cur_node->long_name.size() + 1));
                            }
                            if (cur_node->narg != kDelayedOptional && cur_node->narg != kImmediateOptional) {
                                cur_node->to_read = true;
                                is_involved.push_back(cur_node);
                            }
                        }
                    }
                } 
                else if (string.size() == cur_node->long_name.size()) {
                    if (cur_node->long_name == string) {
                        is_reading[index] = true;
                        cur_node->is_involved = true;
                        if (cur_node->narg != kCommandOnly) {
                            cur_node->to_read = true;
                            is_involved.push_back(cur_node);
                        }
                    }
                }
                cur_node = cur_node->next_node;
            }
            
            //Если это была команда, то выключаем считывания для старых Аргументов
            if (is_reading[index]) {
                cur_node = start_node_;
                size_t index_involved = 0;
                while (cur_node != nullptr) {
                    if ((cur_node->narg == kImmediateOptional|| cur_node->narg == kImmediateZeroOrMore)
                        && (index_involved == is_involved.size() || cur_node != is_involved[index_involved])) {
                        cur_node->to_read = false;
                    } 
                    else if (index_involved != is_involved.size() && cur_node == is_involved[index_involved]) {
                        ++index_involved;
                    }
                    cur_node = cur_node->next_node;
                }
                continue;
            }
            
            cur_node = start_node_;
            while (cur_node != nullptr) {
                if ((cur_node->narg == kImmediateOptional|| cur_node->narg == kImmediateZeroOrMore)
                    && cur_node->to_read) {
                    is_reading[index] = true;
                    cur_node->vector.push_back(string);
                    if (cur_node->narg == kImmediateOptional) {
                        cur_node->to_read = false;
                    }
                }
                cur_node = cur_node->next_node;
            }
            
            if (is_reading[index]) {
                continue;
            }
            
            cur_node = start_node_;
            while (cur_node != nullptr) {
                if ((cur_node->narg == kDelayedOptional|| cur_node->narg == kDelayedZeroOrMore)
                    && cur_node->to_read) {
                    is_reading[index] = true;
                    cur_node->vector.push_back(string);
                    if (cur_node->narg == kDelayedOptional) {
                        cur_node->to_read = false;
                    }
                }
                cur_node = cur_node->next_node;
            }
            
            if (is_reading[index]) {
                continue;
            }
            
            cur_node = start_node_;
            while (cur_node != nullptr) {
                if (cur_node->narg == kRemainingArguments) {
                    is_reading[index] = true;
                    cur_node->vector.push_back(string);
                }
                cur_node = cur_node->next_node;
            }
        }
    }

    void Parser::AddArgument (const char* small_name, const char* long_name, 
    const char* name, Nargs narg) {
        if (last_node_ == nullptr) {
            last_node_ = new Node(std::string(small_name), std::string(long_name), std::string(name), narg);
            start_node_ = last_node_;
        } else {
            last_node_->next_node = new Node(std::string(small_name), std::string(long_name), std::string(name), narg);
            last_node_ = last_node_->next_node;
        }
        ++size_;
    }

    size_t Parser::Size () {
        return size_;
    }

    bool Parser::ArgumentInvolved(const char* name) {
        if (name == nullptr) {
            return false;
        }
        Node* node_argument = start_node_;
        std::string name_str(name);
        while (node_argument != nullptr && node_argument->name != name_str) {
            node_argument = node_argument->next_node;
        }
        if (node_argument != nullptr) {
            return node_argument->is_involved;
        }
        return false;
    }

    size_t Parser::ArgumentSize (const char* name) {
        if (name == nullptr) {
            return 0;
        }
        Node* node_argument = start_node_;
        std::string name_str(name);
        while (node_argument != nullptr && node_argument->name != name_str) {
            node_argument = node_argument->next_node;
        }
        if (node_argument != nullptr) {
            return node_argument->vector.size();
        }
        return 0;
    }

    const std::string& Parser::GetArgumentValue (const char* name, size_t index) {
        if (name == nullptr) {
            throw Exception("GetArgumentValue попытка передать nullptr", 5);
        }
        Node* node_argument = start_node_;
        std::string name_str(name);
        while (node_argument != nullptr && node_argument->name != name_str) {
            node_argument = node_argument->next_node;
        }
        if (node_argument != nullptr) {
            return node_argument->vector[index];
        }
        throw Exception("GetArgumentValue не нашёл к кому обратиться", 5);
    }

}; // namespace mystd
