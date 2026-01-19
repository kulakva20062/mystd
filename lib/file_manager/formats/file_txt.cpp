#include "file_txt.hpp"

FileTxt::FileTxt(const fs::path& file_path, bool is_open)
    :RegularFile(file_path, is_open)
{}

FileTxt::FileTxt(FileTxt& other, const fs::path& file_path, bool is_open)
    :RegularFile(other, file_path, is_open)
{}

FileTxt& FileTxt::operator=(FileTxt& other)
{
    if (this == &other) 
        return *this;
    RegularFile::operator=(other);
    return *this;
}

FileTxt& FileTxt::operator+=(const std::string& message)
{
    Open();
    auto& data = RegularFile::Data();
    for (char c : message) {
        data.push_back(static_cast<std::byte>(c));
    }
    data.push_back(static_cast<std::byte>('\n'));
    Close();
    return *this;
}

void FileTxt::Clear()
{
    Open();
    RegularFile::Data().resize(0);
    Close();
}