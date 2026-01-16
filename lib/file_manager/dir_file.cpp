#include "dir_file.hpp"
#include "regular_file.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void DirFile::CreateFile() const {
    if (Exists(GetPath())) {
        return;
    }
    fs::create_directories(GetPath());
}

void DirFile::Open() {
    CreateFile();
    if (is_open_) {
        return;
    }

    for (const auto& entry : fs::directory_iterator(GetPath())) {
        if (fs::is_regular_file(entry)) {
            files_.push_back(std::make_unique<RegularFile>(entry.path(), false));
        }
    }
    
    is_open_ = true;
}

void DirFile::Close() {
    if (!is_open_) {
        return;
    }

    for (auto& file : files_) {
        file->Close();
    }

    files_.resize(0);
    is_open_ = false;
}

bool DirFile::IsOpen() const {
    return is_open_;
}

void DirFile::CheckOpen() {
    if (is_open_) {
        Open();
    }
}

void DirFile::CopyFile(DirFile& other_file) {
    bool start_open = other_file.IsOpen();
    other_file.Open();
    CreateFile();

    for (auto& file : other_file.Data()) {
        files_.push_back(std::make_unique<RegularFile>(*file, file->GetPath(), false));
    }

    if(!start_open) {
        other_file.Close();
    }
}

void DirFile::DeleteFile() {
    if(!Exists(GetPath())) return;
    Close();
    fs::remove_all(GetPath());
}

std::vector<std::unique_ptr<RegularFile>>& DirFile::Data() {
    is_open_ = true;
    Open();
    return files_;
}

DirFile::DirFile(const fs::path& file_path, bool is_open)
    :File(file_path)
    ,is_open_(is_open)
{
    CreateFile();
    CheckOpen();
}

DirFile::~DirFile() {
    Close();
}

DirFile::DirFile(DirFile& other, const fs::path& file_path, bool is_open)
    :File(file_path)
    ,is_open_(is_open)
{
    DeleteFile();
    CreateFile();
    CopyFile(other);
    is_open_ = is_open;
    CheckOpen();
}

DirFile& DirFile::operator=(DirFile& other) {
    DeleteFile();
    CreateFile();
    CopyFile(other);
    return *this;
}
