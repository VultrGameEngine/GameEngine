#pragma once
#include <string>
#include "file.h"
#include <set>
#include <filesystem>

namespace Vultr
{
    class Directory
    {
      public:
        Directory(const std::string &p_path);
        ~Directory() = default;
        std::filesystem::path GetPath() const;

        Directory CreateSubDirectory(const std::string &name);
        File CreateFile(const std::string &name);

        std::set<File> GetFiles()
        {
            if (!cached_files)
                CacheFiles();
            return std::set(files.begin(), files.end());
        }

        std::vector<File> Files()
        {
            if (!cached_files)
                CacheFiles();
            return files;
        }

        std::vector<Directory> Directories()
        {
            if (!cached_files)
                CacheFiles();
            return sub_directories;
        }

        std::set<Directory> GetDirectories()
        {
            if (!cached_files)
                CacheFiles();
            return std::set(sub_directories.begin(), sub_directories.end());
        }

        std::string GetName() const
        {
            return path.filename().string();
        }

        void Delete();
        Directory operator/(const std::string &dir_name)
        {
            return Directory((path / dir_name).string());
        }

        Directory operator/(const Directory &dir)
        {
            return Directory((path / dir.path).string());
        }

        File operator/(const File &file)
        {
            std::filesystem::path new_path = path / file.GetName();
            return File(new_path.string());
        }

        void CacheFiles();

      private:
        std::filesystem::path path;
        std::vector<File> files;
        std::vector<Directory> sub_directories;
        bool cached_files = false;
        friend bool operator<(const Directory &a, const Directory &b);
        friend bool operator==(const Directory &a, const Directory &b);
    };
    inline bool operator<(const Directory &a, const Directory &b)
    {
        return a.path.string() < b.path.string();
    }
    inline bool operator==(const Directory &a, const Directory &b)
    {
        return a.path.string() == b.path.string();
    }
    inline bool operator!=(const Directory &a, const Directory &b)
    {
        return !(a == b);
    }
} // namespace Vultr
