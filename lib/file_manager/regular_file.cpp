#include "regular_file.hpp"
#include "../exception.hpp"
#include <cstddef>
#include <cstdio>
#include <filesystem> 
#include <vector>

namespace fs = std::filesystem;

RegularFile::~RegularFile() {
    Close();
}

fs::path RegularFile::GetStem() const {
    return GetPath().stem();
}

fs::path RegularFile::GetExtension() const {
    return GetPath().extension();
}

void RegularFile::CreateFile() const {
    if(Exists(GetPath())) return;
    fs::path parent_path = GetParentPath();
    if(!parent_path.empty()) {
        fs::create_directories(parent_path);
    }
    FILE* file = fopen(GetPath().string().c_str(), "ab");
    if(!file) {
        throw mystd::Exception("Не удалось создать несуществующий файл, class RegularFile", 0);
    }
    fclose(file);
}

void RegularFile::DeleteFile() {
    if(!Exists(GetPath())) return;
    fs::remove(GetPath());
}

bool RegularFile::IsOpen() const {
    return is_open_;
}

void RegularFile::Open(){
    if(is_open_) return;
    CreateFile();

    FILE* file = fopen(GetPath().string().c_str(), "rb");
    if(!file) {
        throw mystd::Exception("Файл был создан, открыть его не удалось, возможно он был удалён, class RegularFile", 1);
    }
    std::fseek(file, 0, SEEK_END);
    size_t size = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);


    data_.resize(size);
    fread(data_.data(), sizeof(std::byte), size, file);

    fclose(file);
    is_open_ = true;
}

void RegularFile::Close() {
    if (!is_open_) return;

    DeleteFile();
    CreateFile();
    FILE* file = fopen(GetPath().string().c_str(), "wb");
    if(!file) {
        throw mystd::Exception("Файл был создан, открыть его не удалось, возможно он был удалён, class RegularFile", 1);
    }
    fwrite(data_.data(), sizeof(std::byte), data_.size(), file);
    fclose(file);
    data_.resize(0);
    is_open_ = false;
}

std::vector<std::byte>& RegularFile::Data() {
    Open();
    return data_;
}

void RegularFile::CopyFile(RegularFile& other_file){
    bool start_open = other_file.IsOpen();
    other_file.Open();
    CreateFile();
    FILE* file = fopen(GetPath().string().c_str(), "wb");
    if(!file) {
        throw mystd::Exception("Файл был создан, открыть его не удалось, возможно он был удалён, class RegularFile", 1);
    }

    fwrite(other_file.Data().data(), sizeof(std::byte), other_file.Data().size(), file);

    fclose(file);
    if(!start_open) {
        other_file.Close();
    }
}

RegularFile::RegularFile(const fs::path& file_path, bool is_open) 
    :File(file_path)
    ,is_open_(false)
{
    if(is_open) {
        Open();
    }
}

RegularFile::RegularFile(RegularFile& other, const fs::path& file_path, bool is_open) 
    :File(file_path)
    ,is_open_(false)
{
    DeleteFile();
    CopyFile(other);
    if(is_open) {
        Open();
    }
}

RegularFile& RegularFile::operator=(RegularFile& other) {
    Close();
    DeleteFile();
    CopyFile(other);
    return *this;
}

RegularFile& RegularFile::operator+=(const std::vector<std::byte>& message) {
    CreateFile();
    FILE* file = fopen(GetPath().string().c_str(), "ab");
    if(!file) {
        throw mystd::Exception("Файл был создан, открыть его не удалось, возможно он был удалён, class RegularFile", 1);
    }
    fwrite(message.data(), sizeof(std::byte), message.size(), file);
    fclose(file);
    return *this;
}

RegularFile& RegularFile::operator+=(const std::string& message) {
    CreateFile();
    FILE* file = fopen(GetPath().string().c_str(), "ab");
    if(!file) {
        throw mystd::Exception("Файл был создан, открыть его не удалось, возможно он был удалён, class RegularFile", 1);
    }
    fwrite(message.c_str(), sizeof(char), message.size(), file);
    fclose(file);
    return *this;
}

