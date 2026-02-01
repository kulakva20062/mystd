#pragma once
#include "../regular_file.hpp"
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

std::vector<std::string> LineToWords(const std::string& line);

class FileTxt : public RegularFile {
    public:
        FileTxt(const fs::path& file_path);

        FileTxt(std::string_view text, const fs::path& file_path);

        virtual ~FileTxt() override;

        FileTxt(FileTxt& other, const fs::path& file_path);

        FileTxt& operator=(FileTxt& other);

        FileTxt& operator=(std::string_view text);

        FileTxt& operator+=(std::string_view message);

        void Clear();

        void DeleteComments(const std::string& comment_chars = "#");

        std::string GetData();

        std::vector<std::string> GetLines();

        std::vector<std::string> GetWords();
};

