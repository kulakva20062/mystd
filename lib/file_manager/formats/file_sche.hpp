//cspell:disable
//формат файла создал для личного проекта, не имеет ничего общего с какими-либо стандартами
#pragma once
#include "file_txt.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace mystd {

    class FileSche final : protected FileTxt {
        public:
            FileSche (const fs::path& file_path);

            FileSche (FileSche& other, const fs::path& file_path);

            ~FileSche() override = default;

            FileSche& operator=(FileSche& other);

            
    };

} // namespace mystd
