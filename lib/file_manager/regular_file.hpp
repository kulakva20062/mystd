#include "file.hpp"
#include <cstddef>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class RegularFile : public File {
    public:
        RegularFile(const fs::path& file_path, bool is_open = false);

        ~RegularFile() = default;

        RegularFile(RegularFile& other, const fs::path& file_path, bool is_open = false); 

        RegularFile& operator=(RegularFile& other);

        fs::path GetStem() const;

        fs::path GetExtension() const;

        void Open();

        void Close();

        const std::vector<std::byte>& Data();

        bool IsOpen();

    private:
        bool is_open_;
        std::vector<std::byte> data_;

        void CheckOpen();

        void CreateFile() const;

        void DeleteFile();

        void CopyFile(RegularFile& other_file) const;
};