#pragma once
#include "../regular_file.hpp"
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace mystd {

    class FileSav : public RegularFile {
        public:
            FileSav(const fs::path& file_path);

            virtual ~FileSav() override;

            FileSav(FileSav& other, const fs::path& file_path);



        private:

    };

} // namespace mystd