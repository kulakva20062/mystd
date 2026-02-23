#include "file_sav.hpp"

namespace mystd {

    FileSav::FileSav(const fs::path& file_path)
        :RegularFile(file_path)
    {}

    FileSav::~FileSav()
    {
        Close();
    }
    
    FileSav::FileSav(FileSav& other, const fs::path& file_path) 
        :RegularFile(other, file_path)
    {}

} // namespace mystd