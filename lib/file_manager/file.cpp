#include "file.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool Exists(fs::path file_path) {
    return fs::exists(file_path);
}

bool IsDirectory(fs::path file_path) {
    return fs::is_directory(file_path);
}

bool IsRegularFile(fs::path file_path) {
    return fs::is_regular_file(file_path);
}

File::File(const fs::path& file_path) 
    :file_path_(file_path)
{}

const fs::path& File::GetPath() const {
    return file_path_;
}

fs::path File::GetFileName() const {
    return file_path_.filename();
}

fs::path File::GetParentPath() const {
    return file_path_.parent_path();
}

void File::SetPath(const fs::path& new_file_path) {
    file_path_ = new_file_path;
}

