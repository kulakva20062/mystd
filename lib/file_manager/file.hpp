#pragma once
#include <filesystem> 

namespace fs = std::filesystem;

bool Exists(fs::path);

bool IsDirectory(fs::path);

bool IsRegularFile(fs::path);

class File {
    public:
        File(const fs::path& file_path);

        virtual ~File() = default;

        File(const File& other) = delete;

        File& operator=(const File& other) = delete;

        const fs::path& GetPath() const;

        fs::path GetFileName() const;

        fs::path GetParentPath() const;

        virtual void DeleteFile() = 0;

        virtual void Close() = 0;

        virtual void Open() = 0;

        virtual bool IsOpen() const = 0;

    protected:
        void SetPath(const fs::path& new_file_path);

    private:
        fs::path file_path_;
};