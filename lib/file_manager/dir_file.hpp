#pragma once
#include "file.hpp"
#include "regular_file.hpp"
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class DirFile : public File {
    public:
        DirFile(const fs::path& file_path, bool is_open = false);

        virtual ~DirFile() override;

        DirFile(DirFile& other, const fs::path& file_path, bool is_open = false);

        DirFile& operator=(DirFile& other);

        void DeleteFile() override;

        void Open() override;

        void Close() override;

        bool IsOpen() const override;

        std::vector<std::unique_ptr<RegularFile>>& Data();

    private:
        bool is_open_;
        std::vector<std::unique_ptr<RegularFile>> files_;

        void CheckOpen();

        void CreateFile() const;

        void CopyFile(DirFile& other_file);
        
};