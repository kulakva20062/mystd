#pragma once
#include "containers/string.hpp"
#include "containers/vector.hpp"
#include <cstddef>

namespace mystd {

    enum Nargs {
        kImmediateOptional = 0,     // Немедленная, необязательная
        kDelayedOptional = 1,       // Отложенная, необязательная  
        kDelayedZeroOrMore = 2,     // Отложенная, ноль или более
        kImmediateZeroOrMore = 3,   // Немедленная, ноль или более
        kCommandOnly = 4,           // Только команда
        kRemainingArguments = 5     // Всё оставшееся
    };
 
    //Нет конструктора копирования и оператора присвоить,
    //потому что передавались бы указатели, а это просто не нужно,
    //изначально не задумывалось, что этот класс можно будет копировать, присваивать.
    class Parser final {
        public:
            Parser();

            ~Parser();

            Parser(const Parser&) = delete;

            Parser& operator=(const Parser&) = delete;

            void Parse(int argc, const char** argv);

            void AddArgument (const char* small_name, const char* long_name, 
            const char* name, Nargs narg);

            size_t Size ();

            bool ArgumentInvolved(const char* name);

            size_t ArgumentSize (const char* name);

            const String& GetArgumentValue (const char* name, size_t index);

        private:
            struct Node {
                Vector<String> vector;

                const String small_name;
                const String long_name;
                const String name;
                Nargs narg;
                Node* next_node;
                bool is_involved;
                bool to_read;
                
                Node(const String& small_name, const String& long_name, const String& name, Nargs narg)
                    : vector()
                    , small_name(small_name)
                    , long_name(long_name)
                    , name(name)
                    , narg(narg)
                    , next_node(nullptr)
                    , is_involved(false)
                    , to_read(false)
                {}
            };
            Node* start_node_;
            Node* last_node_;
            size_t size_;
    };

}; // namespace mystd