#pragma once
#include "../regular_file.hpp"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileTxt final : protected RegularFile {
    public:
        FileTxt(const fs::path& file_path, bool is_open = false);

        ~FileTxt() = default;

        FileTxt(FileTxt& other, const fs::path& file_path, bool is_open = false);

        FileTxt& operator=(FileTxt& other);

        FileTxt& operator+=(const std::string& message);

        void Clear();
        
};

