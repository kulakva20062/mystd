#pragma once
#include "file_txt.hpp"

class FileLog final : public FileTxt {
    public:
        FileLog(const fs::path& file_path) 
            :FileTxt(file_path)
        {}

        FileLog(std::string_view text, const fs::path& file_path) 
            :FileTxt(text, file_path)
        {}

        ~FileLog() override {
            Close();
        }

        FileLog(FileLog& other, const fs::path& file_path) 
            :FileTxt(other, file_path)
        {}

        FileLog& operator=(FileLog& other) {
            if(this == &other) {
                return *this;
            }
            FileTxt::operator=(other);
            return *this;
        }
};