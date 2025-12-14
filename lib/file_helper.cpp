#include "file_helper.hpp"
#include "string.hpp"
#include "vector.hpp"
#include <fstream>

namespace mystd {

    ReaderBuffer::ReaderBuffer(const char* file_name, const size_t buffer_size)
        :file_name(file_name)
        ,buffer(buffer_size)
    {
        file.open(file_name);
    }

    ReaderBuffer::~ReaderBuffer() {
        file.close();
    }
    
    bool ReaderBuffer::Read() {
        ReadBuffer();
        if(buffer.Size() == 0) return false;
        return true;
    }
    
    void ReaderBuffer::Resize(const size_t new_size) {
        buffer.Resize(new_size);
    }
    
    void ReaderBuffer::ReadBuffer() {
        file.read(buffer.Data(), buffer.Size());
        std::streamsize bytesRead = file.gcount(); 
        buffer.Resize(bytesRead);
    }
    
    const Vector<char>& ReaderBuffer::GetBuffer() const {
        return buffer;
    }
    
    ReaderString::ReaderString(const char* file_name, char comments)
        :file_name(file_name)
        ,comments(comments)
    {
        file.open(file_name);
    }

    ReaderString::~ReaderString() {
        file.close();
    }
    
    bool ReaderString::Read() {
        ReadString();
        if (string.Size() == 0) return false;
        return true;
    }
    
    void ReaderString::ReadString() {
        file >> string;
        if (string.Size() == 0) return;
        while (string[0] == comments) {
            while (string[string.Size() - 1] != '\n') {
                file >> string;
                if (string.Size() == 0) return;
            }
            file >> string;
            if (string.Size() == 0) return;
        }
        if (string[string.Size() - 1] == '\n') {
            string.Resize(string.Size() - 1);
        }
    }
    
    String& ReaderString::GetString() {
        return string;
    }
    
    WriterBuffer::WriterBuffer(const char* file_name, const size_t buffer_size)
        :file_name(file_name)
        ,buffer()
        ,size(buffer_size)
    {
        file.open(file_name);
    }
    
    WriterBuffer::~WriterBuffer() {
        Write();
        file.close();
    }
    
    void WriterBuffer::Write(const Vector<char>& data) {
        buffer += data;
        if (buffer.Size() > size) {
            Write();
        }
    }
    
    void WriterBuffer::Resize(const size_t new_size) {
        Write();
        size = new_size;
    }
    
    void WriterBuffer::Write() {
        file.write(buffer.Data(), buffer.Size());
        buffer.Resize(0);
    }
    
} // namespace mystd
