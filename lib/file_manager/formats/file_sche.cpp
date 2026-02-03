//cspell:disable
//формат файла создал для личного проекта, не имеет ничего общего с какими-либо стандартами
#include "file_sche.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace mystd {

    FileSche::FileSche(const fs::path& file_path)
        :FileTxt(file_path)
    {}

    FileSche::FileSche(FileSche& other, const fs::path& file_path)
        :FileTxt(other, file_path)
    {}

    FileSche& FileSche::operator=(FileSche& other)
    {
        if (this == &other) return *this;
        FileTxt::operator=(other);
        return *this;
    }
    
} // namespace mystd