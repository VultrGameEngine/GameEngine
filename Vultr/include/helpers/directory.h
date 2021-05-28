#pragma once
#include <string>
#include "file.h"
#include <filesystem>

namespace Vultr
{
    struct Directory
    {
        Directory() = default;
        Directory(const std::string &path = "");
        std::filesystem::path path;
        std::vector<File> files;
        std::vector<Directory> sub_directories;
        bool cached_files = false;
    };

    std::string directory_get_name(const Directory &dir);
    u32 directory_get_number_files(Directory &dir, bool use_cache = true);
    std::vector<File> directory_get_files(Directory &dir, bool use_cache = true);
    std::vector<Directory> directory_get_sub_directories(Directory &dir, bool use_cache = true);

    bool delete_directory(Directory &dir);
    void rename_directory(Directory &dir, const char *name);

    Directory create_sub_directory(Directory &dir, const char *name);
    File create_file(Directory &dir, const char *name);

    // Move a file to a directory
    void move_file(File &file, Directory &dir);

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
    inline Directory operator/(const Directory &dir, const std::string &dir_name)
    {
        return Directory((dir.path / dir_name).string());
    }

    inline Directory operator/(const Directory &dir1, const Directory &dir2)
    {
        return Directory((dir1.path / dir2.path).string());
    }

    inline File operator/(const Directory &dir, const File &file)
    {
        std::filesystem::path new_path = dir.path / file_get_name(file);
        return File(new_path.string());
    }
} // namespace Vultr
