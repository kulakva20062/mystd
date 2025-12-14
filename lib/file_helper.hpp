#pragma once
#include "vector.hpp"
#include "string.hpp"
#include <fstream>

namespace mystd {
    
    class ReaderBuffer {
        public:
            ReaderBuffer(const char* file_name, const size_t buffer_size = 4096);
            
            ~ReaderBuffer();
            
            ReaderBuffer(const ReaderBuffer& other) = delete;
            
            ReaderBuffer& operator=(const ReaderBuffer& other) = delete;
            
            bool Read();
            
            void Resize(const size_t new_size);
            
            const Vector<char>& GetBuffer() const;
                    
        private:
            std::ifstream file;
            const char* file_name;
            Vector<char> buffer;
            
            void ReadBuffer();
    };
    
    class ReaderString {
        public:
            ReaderString(const char* file_name, char comments = '\n');
            
            ~ReaderString();
            
            ReaderString(const ReaderString& other) = delete;
            
            ReaderString& operator=(const ReaderString& other) = delete;
            
            bool Read();
            
            String& GetString();
                    
        private:
            std::ifstream file;
            const char* file_name;
            char comments;
            String string;
            
            void ReadString();
    };
    
    class WriterBuffer {
        public:
            WriterBuffer(const char* file_name, const size_t buffer_size = 4096);
            
            ~WriterBuffer();
            
            WriterBuffer(const WriterBuffer& other) = delete;
            
            WriterBuffer& operator=(const WriterBuffer& other) = delete;
            
            void Write(const Vector<char>& data);
            
            void Resize(const size_t new_size);
                    
        private:
            std::ofstream file;
            const char* file_name;
            Vector<char> buffer;
            size_t size;
            
            void Write();
    };
    
} // namespace mystd