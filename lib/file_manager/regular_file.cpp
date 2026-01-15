#include "regular_file.hpp"
#include "exception.hpp"
#include <cstddef>
#include <cstdio>
#include <filesystem> 
#include <vector>

namespace fs = std::filesystem;

fs::path RegularFile::GetStem() const {
    return GetPath().stem();
}

fs::path RegularFile::GetExtension() const {
    return GetPath().extension();
}

void RegularFile::CreateFile() const {
    if(Exists(GetPath())) return;
    fs::create_directories(GetParentPath());
    FILE* file = fopen(GetFileName().c_str(), "ab");
    if(!file) {
        throw mystd::Exception("Не удалось создать несуществующий файл, class RegularFile", 0);
    }
    fclose(file);
}

void RegularFile::DeleteFile() {
    if(!Exists(GetPath())) return;
    Close();
    fs::remove(GetPath());
}

bool RegularFile::IsOpen() {
    return is_open_;
}

void RegularFile::Open(){
    if (is_open_) return;
    FILE* file = fopen(GetPath().c_str(), "rb");
    if(!file) {
        throw mystd::Exception("Файл был создан, открыть его не удалось, возможно он был удалён, class RegularFile", 1);
    }
    is_open_ = true;

    std::fseek(file, 0, SEEK_END);
    size_t size = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);


    data_.resize(size);
    fread(data_.data(), sizeof(std::byte), size, file);

    fclose(file);
}

void RegularFile::Close() {
    is_open_ = false;
    data_.resize(0);
}

void RegularFile::CheckOpen() {
    if (is_open_) {
        Open();
    }
}

const std::vector<std::byte>& RegularFile::Data() {
    if (!is_open_) {
        Open();
    }
    return data_;
}

void RegularFile::CopyFile(RegularFile& other_file) const{
    bool start_open = other_file.IsOpen();
    other_file.Open();
    FILE* file = fopen(GetPath().c_str(), "wb");
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
    ,is_open_(is_open)
{
    CreateFile();
    CheckOpen();
}

RegularFile::RegularFile(RegularFile& other, const fs::path& file_path, bool is_open) 
    :File(file_path)
    ,is_open_(is_open)
{
    DeleteFile();
    CreateFile();
    CopyFile(other);
    is_open_ = is_open;
    CheckOpen();
}

RegularFile& RegularFile::operator=(RegularFile& other) {
    DeleteFile();
    CreateFile();
    CopyFile(other);
    return *this;
}



