#pragma once
#include "../regular_file.hpp"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileTxt final : protected RegularFile {
    public:
        FileTxt(const fs::path& file_path);

        ~FileTxt() = default;

        FileTxt(FileTxt& other, const fs::path& file_path);

        FileTxt& operator=(FileTxt& other);

        FileTxt& operator+=(const std::string& message);

        void Clear();

        void DeleteFile();

        void DeleteComments(const std::string& comment_chars = "#");

        std::string GetData();

        std::vector<std::string> GetLines();

        std::vector<std::string> GetWords();
};

