#pragma once
#include <filesystem> 

namespace fs = std::filesystem;

bool Exists(fs::path);

bool IsDirectory(fs::path);

bool IsRegularFile(fs::path);

class File {
    public:
        File(const fs::path& file_path);

        ~File() = default;

        File(const File& other) = delete;

        File& operator=(const File& other) = delete;

        const fs::path& GetPath() const;

        fs::path GetFileName() const;

        fs::path GetParentPath() const;

        virtual void DeleteFile();

        virtual void Close();

        virtual void Open();

        virtual bool IsOpen() const;

    protected:
        void SetPath(const fs::path& new_file_path);

    private:
        fs::path file_path_;
};