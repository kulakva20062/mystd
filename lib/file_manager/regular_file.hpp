#pragma once
#include "file.hpp"
#include <cstddef>
#include <vector>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

namespace mystd {

    class RegularFile : public File {
        public:
            RegularFile(const fs::path& file_path, bool is_open = false);

            virtual ~RegularFile() override;

            RegularFile(RegularFile& other, const fs::path& file_path, bool is_open = false); 

            RegularFile& operator=(RegularFile& other);

            RegularFile& operator+=(const std::vector<std::byte>& message);

            RegularFile& operator+=(std::string_view message);

            fs::path GetStem() const;

            fs::path GetExtension() const;

            void DeleteFile() override;

            void Close() override;

        protected:

            void Open() override;

            bool IsOpen() const override;

            std::vector<std::byte>& Data();

        private:
            bool is_open_;
            std::vector<std::byte> data_;

            void CreateFile() const;

            void CopyFile(RegularFile& other_file);
    };

} // namespace mystd