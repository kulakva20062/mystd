#pragma once
#include "file.hpp"
#include "regular_file.hpp"
#include <vector>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

namespace mystd {

class DirFile : public File {
    public:
        DirFile(const fs::path& file_path, bool is_open = false);

        virtual ~DirFile() override;

        DirFile(DirFile& other, const fs::path& file_path, bool is_open = false);

        DirFile& operator=(DirFile& other);

        DirFile& operator+=(const std::vector<std::byte>& message);

        DirFile& operator+=(std::string_view message);

        void DeleteFile() override;

        void Open() override;

        void Close() override;

        bool IsOpen() const override;

        std::vector<std::unique_ptr<RegularFile>>& Data();

    private:
        bool is_open_;
        std::vector<std::unique_ptr<RegularFile>> files_;

        void CreateFile() const;

        void CopyFile(DirFile& other_file);
        
};

} // namespace mystd