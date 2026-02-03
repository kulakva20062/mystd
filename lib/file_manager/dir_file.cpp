#include "dir_file.hpp"
#include "regular_file.hpp"
#include "formats/file_log.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace mystd {

    void DirFile::CreateFile() const {
        if (mystd::Exists(GetPath())) {
            return;
        }
        fs::path path = GetPath();
        if(!path.empty()) {
            fs::create_directories(path);
        }
    }

    void DirFile::Open() {
        if (is_open_) {
            return;
        }

        CreateFile();

        //TODO: Добавить обработку файлов других форматов ( не знаю как сделать универсально )
        for (const auto& entry : fs::directory_iterator(GetPath())) {
            if (fs::is_regular_file(entry)) {
                if (entry.path().extension() == ".log") {
                    files_.push_back(std::make_unique<FileLog>(entry.path()));
                } else if (entry.path().extension() == ".txt") {
                    files_.push_back(std::make_unique<FileTxt>(entry.path()));
                } else {
                    files_.push_back(std::make_unique<RegularFile>(entry.path(), false));
                }
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
        if(!mystd::Exists(GetPath())) return;
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
        ,is_open_(false)
    {
        CreateFile();
        if(is_open) {
            Open();
        }
    }

    DirFile::~DirFile() {
        Close();
    }

    DirFile::DirFile(DirFile& other, const fs::path& file_path, bool is_open)
        :File(file_path)
        ,is_open_(false)
    {
        DeleteFile();
        CopyFile(other);
        if(is_open) {
            Open();
        }
    }

    DirFile& DirFile::operator=(DirFile& other) {
        DeleteFile();
        CopyFile(other);
        return *this;
    }

    DirFile& DirFile::operator+=(const std::vector<std::byte>& message) {
        Open();
        for (auto& file : files_) {
            *file += message;
        }
        return *this;
    }

    DirFile& DirFile::operator+=(std::string_view message) {
        Open();
        for (auto& file : files_) {
            *file += message;
        }
        return *this;
    }

} // namespace mystd