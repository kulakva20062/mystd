#include "file_txt.hpp"
#include <cctype>
#include <algorithm>

FileTxt::FileTxt(const fs::path& file_path)
    :RegularFile(file_path)
{}

FileTxt::FileTxt(FileTxt& other, const fs::path& file_path)
    :RegularFile(other, file_path)
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
    RegularFile::operator+=(message + "\n");
    return *this;
}

void FileTxt::DeleteComments(const std::string& comment_chars)
{
    bool is_space = true;
    Open();
    for (size_t index = 0; index < RegularFile::Data().size(); ++index) {
        if (std::isspace(static_cast<char>(RegularFile::Data()[index]))) { 
            is_space = true;
        }
        else {
            if (is_space && index + comment_chars.size() < RegularFile::Data().size()) {
                for (size_t index_comment = 0; index_comment < comment_chars.size(); ++index_comment) {
                    if (static_cast<char>(RegularFile::Data()[index + index_comment]) != comment_chars[index_comment]) {
                        is_space = false;
                        break;
                    }
                }
                if (is_space) {
                    RegularFile::Data().erase(RegularFile::Data().begin() + index, std::find(RegularFile::Data().begin() + index, RegularFile::Data().end(), static_cast<std::byte>('\n')));
                    continue;
                }
            }
            is_space = false;
        }
    }
    Close();
}

void FileTxt::Clear()
{
    Open();
    RegularFile::Data().resize(0);
    Close();
}

void FileTxt::DeleteFile()
{
    RegularFile::DeleteFile();
}

std::string FileTxt::GetData() 
{
    Open();
    std::string data;
    for (size_t index = 0; index < RegularFile::Data().size(); ++index) {
        data.push_back(static_cast<char>(RegularFile::Data()[index]));
    }
    Close();
    return data;
}

std::vector<std::string> FileTxt::GetLines()
{
    Open();
    std::vector<std::string> lines;
    for (size_t index = 0; index < RegularFile::Data().size(); ++index) {
        auto it = std::find(RegularFile::Data().begin() + index, RegularFile::Data().end(), static_cast<std::byte>('\n'));
        lines.push_back(std::string(RegularFile::Data().begin() + index, it));
        index = it - RegularFile::Data().begin();
    }
    Close();
    return lines;
}

std::vector<std::string> FileTxt::GetWords() {
    Open();
    std::vector<std::string> words;
    for (size_t index = 0; index < RegularFile::Data().size(); ++index) {
        if (std::isspace(static_cast<char>(RegularFile::Data()[index]))) continue;
        auto it = RegularFile::Data().begin() + index;
        while (it < RegularFile::Data().end() && !std::isspace(static_cast<char>(*it))) {
            ++it;
        }
        words.push_back(std::string(RegularFile::Data().begin() + index, it));
        index = it - RegularFile::Data().begin();
    }
    Close();
    return words;
}